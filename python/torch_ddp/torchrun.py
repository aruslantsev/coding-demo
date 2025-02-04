import os
from pathlib import Path
from typing import Tuple, Iterator

import torch
import torch.nn as nn
from torch.distributed import init_process_group, destroy_process_group
from torch.utils.data import Dataset, DataLoader
from torch.utils.data.distributed import (
    DistributedSampler,
)  # Distribute data across multiple gpus

from ddp import TrainerDDP, cifar_dataset
from single import prepare_const, cifar_model


# Each process control a single gpu
def ddp_setup_torchrun():
    init_process_group(backend="nccl")


class MyDistributedSampler(DistributedSampler):
    def __iter__(self) -> Iterator:
        indices = list(range(len(self.dataset)))
        indices = indices[self.rank : len(self.dataset) : self.num_replicas]
        return iter(indices)


def cifar_dataloader_ddp_uneven(
    trainset: Dataset,
    testset: Dataset,
    bs: int,
) -> Tuple[DataLoader, DataLoader, DistributedSampler]:
    sampler_train = DistributedSampler(trainset)
    trainloader = DataLoader(
        trainset, batch_size=bs, shuffle=False, sampler=sampler_train, num_workers=8
    )
    testloader = DataLoader(
        testset,
        batch_size=bs,
        shuffle=False,
        sampler=MyDistributedSampler(testset, shuffle=False),
        num_workers=8,
    )

    return trainloader, testloader, sampler_train


class TrainerDDPTorchrun(TrainerDDP):
    def __init__(
        self,
        model: nn.Module,
        trainloader: DataLoader,
        testloader: DataLoader,
        sampler_train: DistributedSampler,
    ) -> None:
        self.gpu_id = int(os.environ["LOCAL_RANK"])
        self.epochs_run = 0
        super().__init__(self.gpu_id, model, trainloader, testloader, sampler_train)

    def _save_snapshot(self, epoch: int):
        snapshot = dict(
            EPOCHS=epoch,
            MODEL_STATE=self.model.state_dict(),
            OPTIMIZER=self.optimizer.state_dict(),
            LR_SCHEDULER=self.lr_scheduler.state_dict(),
        )
        model_path = self.const["trained_models"] / f"snapshot.pt"
        torch.save(snapshot, model_path)

    def _load_snapshot(self, path: str):
        snapshot = torch.load(path, map_location="cpu")
        self.epochs_run = snapshot["EPOCHS"] + 1
        self.model.load_state_dict(snapshot["MODEL_STATE"])
        self.optimizer.load_state_dict(snapshot["OPTIMIZER"])
        self.lr_scheduler.load_state_dict(snapshot["LR_SCHEDULER"])
        print(
            f"[GPU{self.gpu_id}] Resuming training from snapshot at Epoch {snapshot['EPOCHS']}"
        )

    def train(self, max_epochs: int, snapshot_path: str):
        if Path(snapshot_path).exists():
            print("Loading snapshot")
            self._load_snapshot(snapshot_path)

        self.model.train()
        for epoch in range(self.epochs_run, max_epochs):
            # https://pytorch.org/docs/stable/data.html#torch.utils.data.distributed.DistributedSampler
            self.sampler_train.set_epoch(epoch)

            self._run_epoch(epoch)
            # only save once on master gpu
            if self.gpu_id == 0 and epoch % self.const["save_every"] == 0:
                self._save_snapshot(epoch)
        # save last epoch
        self._save_checkpoint(max_epochs - 1)


def main_ddp_torchrun(
    snapshot_path: str,
    final_model_path: str,
):
    ddp_setup_torchrun()

    const = prepare_const()
    train_dataset, test_dataset = cifar_dataset(const["data_root"])
    train_dataloader, test_dataloader, train_sampler = cifar_dataloader_ddp_uneven(
        train_dataset, test_dataset, const["batch_size"]
    )
    model = cifar_model()
    trainer = TrainerDDPTorchrun(
        model=model,
        trainloader=train_dataloader,
        testloader=test_dataloader,
        sampler_train=train_sampler,
    )
    trainer.train(const["total_epochs"], snapshot_path=snapshot_path)
    trainer.test(final_model_path)

    destroy_process_group()  # clean up


if __name__ == "__main__":
    snapshot_path = Path("./trained_models/snapshot.pt")
    final_model_path = Path("./trained_models/CIFAR10_ddp_epoch14.pt")
    main_ddp_torchrun(snapshot_path, final_model_path)
