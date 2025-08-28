import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from collections import Counter

# ------------------------------
# Load dataset
# ------------------------------
data = pd.read_csv("Iris.csv")

# Features: columns 1-4 (ignore col 0 = ID)
X = data.iloc[:, 1:5].values
y = data.iloc[:, 5].values

# Split into train (90%) and test (10%)
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.1, random_state=42, stratify=y
)

# ------------------------------
# Euclidean distance function
# ------------------------------
def euclidean_distance(x1, x2):
    return np.sqrt(np.sum((x1 - x2) ** 2))

# ------------------------------
# KNN implementation from scratch
# ------------------------------
class KNN:
    def __init__(self, k=3):
        self.k = k

    def fit(self, X, y):
        self.X_train = X
        self.y_train = y

    def predict(self, X):
        predictions = [self._predict(x) for x in X]
        return np.array(predictions)

    def _predict(self, x):
        # Compute distances
        distances = [euclidean_distance(x, x_train) for x_train in self.X_train]
        # Sort and get indices of k nearest neighbors
        k_idx = np.argsort(distances)[:self.k]
        k_neighbor_labels = [self.y_train[i] for i in k_idx]
        # Majority vote
        most_common = Counter(k_neighbor_labels).most_common(1)
        return most_common[0][0]

# ------------------------------
# Grid Search for best K
# ------------------------------
best_k = None
best_acc = 0

for k in range(1, 16):  # check K = 1 to 15
    knn = KNN(k=k)
    knn.fit(X_train, y_train)
    preds = knn.predict(X_test)
    acc = np.mean(preds == y_test)
    print(f"K={k}, Accuracy={acc:.3f}")
    if acc > best_acc:
        best_acc = acc
        best_k = k

print("\nBest K:", best_k, "with accuracy:", best_acc)
