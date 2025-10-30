# implementing of a ANN in python useing pytorch by Aaditya Bhatia (23/CS/004)

import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np
from tqdm import tqdm
import matplotlib.pyplot as plt
from sklearn.datasets import make_blobs
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler

class ANN:
    def __init__(self):
        self.model = nn.Sequential(
            nn.Linear(2, 4),
            nn.ReLU(),
            nn.Linear(4, 1)
        )
        self.criterion = nn.MSELoss()
        self.optimizer = optim.SGD(self.model.parameters(), lr=0.001)
        self.loss_history = []

    def train(self, X, y, epochs):
        for epoch in tqdm(range(epochs)):
            self.optimizer.zero_grad()
            outputs = self.model(X)
            loss = self.criterion(outputs, y)
            loss.backward()
            self.optimizer.step()
            self.loss_history.append(loss.item())
        self.plot_loss()

    def plot_loss(self):
        plt.plot(self.loss_history)
        plt.xlabel('Epoch')
        plt.ylabel('Loss')
        plt.title('Training Loss')
        plt.show()

def generate_data(samples):
    X, y = make_blobs(n_samples=samples, centers=2, n_features=2, random_state=43)
    y = y.reshape(-1, 1).astype(np.float32)

    scaler = StandardScaler()
    X = scaler.fit_transform(X).astype(np.float32)

    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=41)

    return (torch.tensor(X_train), torch.tensor(y_train)), (torch.tensor(X_test), torch.tensor(y_test))

if __name__ == "__main__":
    train_data, test_data = generate_data(10000)
    X_train, y_train = train_data
    X_test, y_test = test_data

    ann = ANN()
    ann.train(X_train, y_train, epochs=1000)
    with torch.no_grad():
        predictions = ann.model(X_test)
        test_loss = ann.criterion(predictions, y_test)
        print(f'Test Loss: {test_loss.item()}')
        print(f'Accuracy: {((predictions.round() == y_test).sum().item() / y_test.size(0)) * 100:.2f}%')
        plt.scatter(X_test[:, 0], X_test[:, 1], c=predictions.numpy(), cmap='viridis')
        plt.title('Test Data Predictions')
        plt.show()

