import torch


class SimpleEncoder(torch.nn.Module):
    def __init__(self, num_features: int = 2):
        super(SimpleEncoder, self).__init__()
        self.fc = torch.nn.Linear(in_features=28 * 28, out_features=num_features)
        
    def forward(self, x: torch.tensor) -> torch.tensor:
        x = x.reshape(-1, 28 * 28)
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
        x = x.reshape(-1, 28, 28)
        return x
    

class AutoEncoder(torch.nn.Module):
    def __init__(self, encoder: torch.nn.Module, decoder: torch.nn.Module):
        super(AutoEncoder, self).__init__()
        self.encoder = encoder
        self.decoder = decoder
        
    def forward(self, x: torch.tensor) -> torch.tensor:
        latent = self.encoder(x)
        image = self.decoder(latent)
        return latent, image


class DeepEncoder(torch.nn.Module):
    def __init__(self, num_features: int = 2):
        super(DeepEncoder, self).__init__()
        self.fc0 = torch.nn.Linear(in_features=28 * 28, out_features=num_features * 3)
        self.fc1 = torch.nn.Linear(in_features=num_features * 3, out_features=num_features * 2)
        self.fc2 = torch.nn.Linear(in_features=num_features * 2, out_features=num_features)
        
    def forward(self, x: torch.tensor) -> torch.tensor:
        x = x.reshape(-1, 28 * 28)
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
        x = x.reshape(-1, 28, 28)
        return x