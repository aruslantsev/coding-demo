from typing import Any, List, Union, Tuple

import matplotlib.pyplot as plt
import numpy as np
import torch
import torchvision
from PIL import Image


def plot_lst(img_list: List[Union[Image.Image, np.array]]) -> None:
    fig, ax = plt.subplots(nrows=1, ncols=len(img_list))
    for i, img in enumerate(img_list):
        plt.imshow(img)
        ax[i].imshow(img, cmap="gray")
    plt.show()


def plot_images(
        indexes: List[int],
        model: torch.nn.Module,
        image_transforms: torchvision.transforms,
        mnist: Any):
    images = []
    model_images = []
    for idx in indexes:
        image = image_transforms(mnist[idx][0]).unsqueeze(0)
        initial_image = image.squeeze(0).detach().numpy()[0]
        images.append(initial_image)
        model_image = model(image)[0].squeeze(0).detach().numpy()[0]
        model_images.append(model_image)
    plot_lst(images)
    plot_lst(model_images)


def train_test_model(
        model: torch.nn.Module,
        optimizer: torch.optim.Optimizer,
        loss_fn: torch.nn.Module,
        dataloader: torch.utils.data.DataLoader,
        mode: str = "train"
) -> Tuple[torch.nn.Module, torch.optim.Optimizer, float]:
    if mode == "train":
        model.train()
    else:
        model.eval()
        
    total_loss = 0
    for images, labels in dataloader:
        if mode == "train":
            optimizer.zero_grad()
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
    image_transforms: torchvision.transforms,
    loss_fn: torch.nn.Module,
    mnist: Any,
    trainloader: torch.utils.data.DataLoader,
    testloader: torch.utils.data.DataLoader,
    plot_idx: List[int],
    epochs: int = 30,
) -> Tuple[torch.nn.Module, torch.optim.Optimizer]:
    plot_images(plot_idx, model, image_transforms, mnist)
    model, optimizer, test_loss = train_test_model(model, optimizer, loss_fn, testloader, "test")

    print(f"test loss: {test_loss:.4f}")
    train_losses = []
    test_losses = []
    for epoch in range(epochs):
        model, optimizer, train_loss = train_test_model(model, optimizer, loss_fn, trainloader,
                                                        "train")
        model, optimizer, test_loss = train_test_model(model, optimizer, loss_fn, testloader,
                                                       "test")
        train_losses.append(train_loss)
        test_losses.append(test_loss)
        print(f" Epoch {epoch}, train loss: {train_loss:.4f}, test loss: {test_loss:.4f}")

        if epoch < 3 or (epoch + 1) % 10 == 0:
            plot_images(plot_idx, model, image_transforms, mnist)
            
    plt.plot(train_losses, label="train")
    plt.plot(test_losses, label="test")
    plt.grid()
    plt.legend()
    plt.show()

    return model, optimizer
