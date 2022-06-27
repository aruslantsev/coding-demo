import torch


class SimpleEncoder(torch.nn.Module):
    def __init__(self, num_features=2):
        super(Encoder, self).__init__()
        self.fc = torch.nn.Linear(in_features=28 * 28, out_features=num_features)
        
    def forward(self, x):
        x = x.reshape(-1, 28 * 28)
        x = self.fc(x)
        x = torch.nn.functional.relu(x)
        return x
        

class SImpleDecoder(torch.nn.Module):
    def __init__(self, num_features=2):
        super(Decoder, self).__init__()
        self.fc = torch.nn.Linear(in_features=num_features, out_features=28 * 28)
        
    def forward(self, x):
        x = self.fc(x)
        x = torch.sigmoid(x)
        x = x.reshape(-1, 28, 28)
        return x
    

class AutoEncoder(torch.nn.Module):
    def __init__(self, encoder, decoder):
        super(AutoEncoder, self).__init__()
        self.encoder = encoder
        self.decoder = decoder
        
    def forward(self, x):
        latent = self.encoder(x)
        image = self.decoder(latent)
        return latent, image