from typing import Any, List, Union, Tuple

import matplotlib.pyplot as plt
import numpy as np
import torch
import torchvision
from PIL import Image


class AddGaussianNoise(object):
    def __init__(self, mean=0., std=1.):
        self.std = std
        self.mean = mean
        
    def __call__(self, tensor: torch.tensor) -> torch.tensor:
        return tensor + torch.randn(tensor.size()) * self.std + self.mean
    
    def __repr__(self):
        return self.__class__.__name__ + f'(mean={self.mean}, std={self.std})'


def plot_lst(img_list: List[Union[Image.Image, np.array]]) -> None:
    fig, ax = plt.subplots(nrows=1, ncols=len(img_list))
    for i, img in enumerate(img_list):
        plt.imshow(img)
        ax[i].imshow(img, cmap="gray")
    plt.show()


class ImagePlotter:
    def __init__(self, image_transforms: torchvision.transforms, mnist: Any, indexes: List[int]):
        self.image_transforms = image_transforms
        self.mnist = mnist
        self.indexes = indexes

    def __call__(self, model: torch.nn.Module, device: torch.device):
        images = []
        model_images = []
        for idx in self.indexes:
            image = self.image_transforms(self.mnist[idx][0]).unsqueeze(0).to(device)
            initial_image = image.squeeze(0).detach().cpu().numpy()[0]
            images.append(initial_image)
            model_image = model(image)[0].squeeze(0).detach().cpu().numpy()[0]
            model_images.append(model_image)
        plot_lst(images)
        plot_lst(model_images)


def train_test_model(
    model: torch.nn.Module,
    optimizer: torch.optim.Optimizer,
    loss_fn: torch.nn.Module,
    dataloader: torch.utils.data.DataLoader,
    device: torch.device,
    mode: str = "train",
    add_noise: bool = False,
) -> Tuple[torch.nn.Module, torch.optim.Optimizer, float]:
    if mode == "train":
        model.train()
    else:
        model.eval()
        
    total_loss = 0
    for images, labels in dataloader:
        images = images.to(device)
        if mode == "train":
            optimizer.zero_grad()
        if add_noise:
            model_images, _ = model(images + torch.randn(images.size()))
        else:
            model_images, _ = model(images)
        loss = loss_fn(model_images, images)
        if mode == "train":
            loss.backward()
            optimizer.step()
            
        total_loss += loss.item()
        
    return model, optimizer, total_loss


def train_model(
    model: torch.nn.Module,
    optimizer: torch.optim.Optimizer,
    loss_fn: torch.nn.Module,
    trainloader: torch.utils.data.DataLoader,
    testloader: torch.utils.data.DataLoader,
    device: torch.device,
    epochs: int = 30,
    image_plotter: ImagePlotter = None,
    add_noise: bool = False,
) -> Tuple[torch.nn.Module, torch.optim.Optimizer]:
    image_plotter(model, device)
    model, optimizer, test_loss = train_test_model(model, optimizer, loss_fn, testloader, device, "test", add_noise=add_noise)

    print(f"test loss: {test_loss:.4f}")
    train_losses = []
    test_losses = []
    for epoch in range(epochs):
        model, optimizer, train_loss = train_test_model(
            model, optimizer, loss_fn, trainloader, device, "train", add_noise=add_noise)
        model, optimizer, test_loss = train_test_model(
            model, optimizer, loss_fn, testloader, device, "test", add_noise=add_noise)
        train_losses.append(train_loss)
        test_losses.append(test_loss)
        print(f" Epoch {epoch}, train loss: {train_loss:.4f}, test loss: {test_loss:.4f}")

        if epoch < 3 or (epoch + 1) % 10 == 0:
            image_plotter(model, device)
            
    plt.plot(train_losses, label="train")
    plt.plot(test_losses, label="test")
    plt.grid()
    plt.legend()
    plt.show()

    return model, optimizer
