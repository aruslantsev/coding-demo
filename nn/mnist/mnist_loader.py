from typing import Tuple, Optional, List

import numpy as np
import torch
import torchvision
from torch.utils.data import Dataset

np.random.seed(42)
torch.random.manual_seed(42)


class MNISTDataset(Dataset):
    def __init__(self, lst, mnist, image_transforms: torchvision.transforms):
        self.lst = lst
        self.mnist = mnist
        self.transforms = image_transforms
        
    def __len__(self) -> int:
        return len(self.lst)
    
    def __getitem__(self, idx) -> Tuple[torch.tensor, torch.tensor]:
        img, label = self.mnist[self.lst[idx]]
        img = self.transforms(img).squeeze(0)
        label = torch.tensor(label)
        return img, label


def train_test_split(
        train_ratio: int = 0.8,
        mnist: Optional[torchvision.datasets] = None
) -> Tuple[List[int], List[int], List[int], List[int]]:

    all_images = list(range(len(mnist['targets'])))
    train_idx = sorted(
        np.random.choice(all_images,
                         int(train_ratio * len(mnist['targets'])), replace=False)
        .tolist()
    )
    test_idx = [image for image in all_images if image not in train_idx]
    
    test_shuffled = test_idx.copy()
    np.random.shuffle(test_shuffled)
    representations = []
    labels = []

    for idx in test_shuffled:
        label = mnist[idx][1]
        if label not in labels:
            representations.append(idx)
            labels.append(label)

    return train_idx, test_idx, representations, labels
