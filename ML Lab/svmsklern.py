import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import make_blobs
from sklearn import svm as sk_svm
from sklearn.metrics import accuracy_score
import pandas as pd

X, y01 = make_blobs(centers=2, cluster_std=0.9, random_state=0, n_samples=150)
y_pm1 = np.where(y01 == 1, 1, -1)

def plot_decision_boundary(model, X, y, title):
    plt.figure()
    plt.scatter(X[:, 0], X[:, 1], c=y, s=50)
    plt.title(title)

    ax = plt.gca()
    xlim = ax.get_xlim()
    ylim = ax.get_ylim()

    xx = np.linspace(xlim[0], xlim[1], 200)
    yy = np.linspace(ylim[0], ylim[1], 200)
    YY, XX = np.meshgrid(yy, xx)
    grid = np.vstack([XX.ravel(), YY.ravel()]).T
    Z = model.decision_function(grid).reshape(XX.shape)

    ax.contour(XX, YY, Z, levels=[0], linestyles="--")
    ax.contour(XX, YY, Z, levels=[-1, 1], linestyles=":")
    plt.show()


svc = sk_svm.SVC(kernel="linear", C=1.0, random_state=0)
svc.fit(X, y01)

class SVM:
    def __init__(self, lr=1e-2, reg=1e-2, epochs=200, batch_size=32, random_state=None, verbose=False):
        self.lr = lr
        self.reg = reg
        self.epochs = epochs
        self.batch_size = batch_size
        self.random_state = random_state
        self.verbose = verbose
        self.W = None
        self.b = 0.0

    @staticmethod
    def _ensure_labels_pm1(y):
        y = np.asarray(y).reshape(-1)
        if set(np.unique(y)).issubset({0, 1}):
            y = np.where(y == 1, 1, -1)
        return y.astype(float)

    def _loss_with_params(self, X, Y, W, b):
        scores = X.dot(W) + b
        margins = 1.0 - Y * scores
        hinge = np.maximum(0.0, margins)
        data_loss = hinge.mean()
        reg_loss = 0.5 * self.reg * np.sum(W * W)
        return data_loss + reg_loss

    def calculate_loss(self, X, Y):
        if self.W is None:
            raise ValueError("Model weights are not set yet; call fit() first.")
        X = np.asarray(X, dtype=float)
        Y = self._ensure_labels_pm1(Y)
        return self._loss_with_params(X, Y, self.W, self.b)

    def fit(self, X, Y):
        X = np.asarray(X, dtype=float)
        Y = self._ensure_labels_pm1(Y)

        m, n = X.shape
        rng = np.random.default_rng(self.random_state)

        w = np.zeros(n, dtype=float)
        b = 0.0

        loss_list = []

        for epoch in range(self.epochs):
            order = rng.permutation(m)
            Xs = X[order]
            Ys = Y[order]

            for start in range(0, m, self.batch_size):
                end = min(start + self.batch_size, m)
                Xb = Xs[start:end]
                Yb = Ys[start:end]
                B = Xb.shape[0]

                scores = Xb.dot(w) + b
                margins = 1.0 - Yb * scores
                mask = margins > 0.0

                if np.any(mask):
                    # gradient of data hinge loss w.r.t w and b
                    gradw_data = -(Xb[mask].T @ Yb[mask]) / B
                    gradb_data = -np.sum(Yb[mask]) / B
                else:
                    gradw_data = np.zeros_like(w)
                    gradb_data = 0.0

                # total gradients (L2 reg gradient = reg * w)
                gradw = self.reg * w + gradw_data
                gradb = gradb_data

                # SGD step
                w -= self.lr * gradw
                b -= self.lr * gradb

            # track loss
            loss = self._loss_with_params(X, Y, w, b)
            loss_list.append(loss)
            if self.verbose and (epoch % max(1, self.epochs // 10) == 0 or epoch == self.epochs - 1):
                print(f"epoch {epoch+1}/{self.epochs}  loss={loss:.6f}")

        self.W = w
        self.b = b
        return w, b, loss_list

    def decision_function(self, X):
        X = np.asarray(X, dtype=float)
        if self.W is None:
            raise ValueError("Model not trained yet; call fit() first.")
        return X.dot(self.W) + self.b

    def predict(self, X):
        return np.where(self.decision_function(X) >= 0.0, 1, -1)

custom_svm = SVM(lr=5e-3, reg=1e-2, epochs=400, batch_size=32, random_state=0, verbose=False)
_, _, loss_hist = custom_svm.fit(X, y_pm1)

svc_pred = svc.predict(X)
custom_pred_pm1 = custom_svm.predict(X)
custom_pred = np.where(custom_pred_pm1 == 1, 1, 0)

svc_acc = accuracy_score(y01, svc_pred)
custom_acc = accuracy_score(y01, custom_pred)

plot_decision_boundary(svc, X, y01, "Sklearn SVC (linear)")
class _Adapter:
    def __init__(self, m): self.m = m
    def decision_function(self, X): return self.m.decision_function(X)

plot_decision_boundary(_Adapter(custom_svm), X, y01, "My SVM (hinge + L2, SGD)")

print(f"Sklearn SVC accuracy: {svc_acc:.4f}")
print(f"My SVM accuracy: {custom_acc:.4f}")