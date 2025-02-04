from pathlib import Path
from typing import Union

import torch
import torch.nn as nn
from torch.nn.parallel import DataParallel
from torch.utils.data import DataLoader

from single import TrainerSingle, prepare_const, cifar_dataloader_single, cifar_dataset, cifar_model


class TrainerDP(TrainerSingle):
    def __init__(
        self,
        model: nn.Module,
        trainloader: DataLoader,
        testloader: DataLoader,
    ):
        self.gpu_id = "cuda"
        super().__init__(self.gpu_id, model, trainloader, testloader)

        self.model = DataParallel(self.model)

    def _save_checkpoint(self, epoch: int):
        ckp = self.model.state_dict()

        model_path = self.const["trained_models"] / f"CIFAR10_dp_epoch{epoch}.pt"
        torch.save(ckp, model_path)


def main_dp(final_model_path: Union[str, Path]):
    const = prepare_const()
    train_dataset, test_dataset = cifar_dataset(const["data_root"])
    train_dataloader, test_dataloader = cifar_dataloader_single(
        train_dataset, test_dataset, const["batch_size"]
    )
    model = cifar_model()
    trainer = TrainerDP(
        model=model,
        trainloader=train_dataloader,
        testloader=test_dataloader,
    )
    trainer.train(const["total_epochs"])
    trainer.test(final_model_path)


if __name__ == "__main__":
    final_model_path = Path("./trained_models/CIFAR10_dp_epoch14.pt")
    main_dp(final_model_path)
