import torch


class SimpleEncoder(torch.nn.Module):
    def __init__(self, num_features: int = 2):
        super(SimpleEncoder, self).__init__()
        self.fc = torch.nn.Linear(in_features=28 * 28, out_features=num_features)
        
    def forward(self, x: torch.tensor) -> torch.tensor:
        x = x.reshape(-1, 1, 28 * 28)
        x = self.fc(x)
        x = torch.nn.functional.relu(x)
        return x
        

class SimpleDecoder(torch.nn.Module):
    def __init__(self, num_features: int = 2):
        super(SimpleDecoder, self).__init__()
        self.fc = torch.nn.Linear(in_features=num_features, out_features=28 * 28)
        
    def forward(self, x: torch.tensor) -> torch.tensor:
        x = self.fc(x)
        x = torch.sigmoid(x)
        x = x.reshape(-1, 1, 28, 28)
        return x
    

class AutoEncoder(torch.nn.Module):
    def __init__(self, encoder: torch.nn.Module, decoder: torch.nn.Module):
        super(AutoEncoder, self).__init__()
        self.encoder = encoder
        self.decoder = decoder
        
    def forward(self, x: torch.tensor) -> torch.tensor:
        latent = self.encoder(x)
        image = self.decoder(latent)
        return image, latent


class DeepEncoder(torch.nn.Module):
    def __init__(self, num_features: int = 2):
        super(DeepEncoder, self).__init__()
        self.fc0 = torch.nn.Linear(in_features=28 * 28, out_features=num_features * 3)
        self.fc1 = torch.nn.Linear(in_features=num_features * 3, out_features=num_features * 2)
        self.fc2 = torch.nn.Linear(in_features=num_features * 2, out_features=num_features)
        
    def forward(self, x: torch.tensor) -> torch.tensor:
        x = x.reshape(-1, 1, 28 * 28)
        x = self.fc0(x)
        x = torch.nn.functional.relu(x)
        x = self.fc1(x)
        x = torch.nn.functional.relu(x)
        x = self.fc2(x)
        x = torch.nn.functional.relu(x)
        return x
        

class DeepDecoder(torch.nn.Module):
    def __init__(self, num_features: int = 2):
        super(DeepDecoder, self).__init__()
        self.fc0 = torch.nn.Linear(in_features=num_features, out_features=num_features * 2)
        self.fc1 = torch.nn.Linear(in_features=num_features * 2, out_features=num_features * 3)
        self.fc2 = torch.nn.Linear(in_features=num_features * 3, out_features=28 * 28)
        
    def forward(self, x: torch.tensor) -> torch.tensor:
        x = self.fc0(x)
        x = torch.nn.functional.relu(x)
        x = self.fc1(x)
        x = torch.nn.functional.relu(x)
        x = self.fc2(x)
        x = torch.sigmoid(x)
        x = x.reshape(-1, 1, 28, 28)
        return x


class ConvAutoEncoder(torch.nn.Module):
    def __init__(self):
        super(ConvAutoEncoder, self).__init__()
        self.conv0 = torch.nn.Conv2d(
            in_channels=1, out_channels=128, kernel_size=(7, 7), padding="same")
        self.conv1 = torch.nn.Conv2d(
            in_channels=128, out_channels=32, kernel_size=(3, 3), padding="same")
        self.conv2 = torch.nn.Conv2d(
            in_channels=32, out_channels=1, kernel_size=(7, 7), padding="same")
        self.pooling = torch.nn.MaxPool2d((2, 2))
        
        self.conv3 = torch.nn.Conv2d(
            in_channels=1, out_channels=32, kernel_size=(7, 7), padding="same")
        self.conv4 = torch.nn.Conv2d(
            in_channels=32, out_channels=128, kernel_size=(3, 3), padding="same")
        self.conv5 = torch.nn.Conv2d(
            in_channels=128, out_channels=1, kernel_size=(7, 7), padding="same")
        self.unpooling = torch.nn.Upsample(scale_factor=(2, 2))

    def forward(self, x: torch.tensor) -> torch.tensor:
        x = self.conv0(x)
        x = torch.nn.functional.relu(x)
        x = self.pooling(x)
        x = self.conv1(x)
        x = torch.nn.functional.relu(x)
        x = self.pooling(x)
        x = self.conv2(x)
        x = torch.nn.functional.relu(x)
        
        x = self.conv3(x)
        x = torch.nn.functional.relu(x)
        x = self.unpooling(x)
        x = self.conv4(x)
        x = torch.nn.functional.relu(x)
        x = self.unpooling(x)
        x = self.conv5(x)
        x = torch.nn.functional.relu(x)
        x = torch.sigmoid(x)
        x = x.reshape(-1, 28, 28)

        return x
