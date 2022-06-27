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
        image = image_transforms(mnist[idx][0])
        images.append(image.detach().numpy()[0])
        model_image = model(image)[1].detach().numpy()[0]
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
        _, model_images = model(images)
        loss = loss_fn(model_images, images)
        if mode == "train":
            loss.backward()
            optimizer.step()
            
        total_loss += loss.item()
        
    return model, optimizer, total_loss
