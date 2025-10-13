import numpy as np

class SVM:
    def __init__(self, lr=1e-2, reg=1e-2, epochs=100, batch_size=32, random_state=None, verbose=False):
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
        scores = X.dot(W) + b                    # includes +b (your correction)
        margins = 1.0 - Y * scores
        hinge = np.maximum(0.0, margins)
        data_loss = hinge.mean()
        reg_loss = 0.5 * self.reg * np.sum(W * W)
        return data_loss + reg_loss

    def calculate_loss(self, X, Y):
        """Loss using the model's stored parameters (after training)."""
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
        bias = 0.0
        lr = self.lr
        reg = self.reg
        Bsize = self.batch_size

        ids = np.arange(n)

        loss_list = []

        for epoch in range(self.epochs):
            order = rng.permutation(m)
            Xs = X[order]
            Ys = Y[order]

            # iterate over all examples (use m)
            for batch_start in range(0, m, Bsize):
                batch_end = min(batch_start + Bsize, m)
                Xb = Xs[batch_start:batch_end]
                Yb = Ys[batch_start:batch_end]
                B = Xb.shape[0]

                scores = Xb.dot(w) + bias
                margins = 1.0 - Yb * scores
                mask = margins > 0.0

                if np.any(mask):
                    gradw_data = -(Xb[mask].T @ Yb[mask]) / B
                    gradb_data = -np.sum(Yb[mask]) / B
                else:
                    gradw_data = np.zeros_like(w)
                    gradb_data = 0.0

                w[ids] = w[ids] - lr * reg * w[ids] + lr * gradw_data[ids]
                bias = bias + lr * gradb_data

            loss = self._loss_with_params(X, Y, w, bias)
            loss_list.append(loss)
            if self.verbose and (epoch % max(1, self.epochs // 10) == 0 or epoch == self.epochs - 1):
                print(f"epoch {epoch+1}/{self.epochs}  loss={loss:.6f}")

        self.W = w
        self.b = bias

        return w, bias, loss_list

    def decision_function(self, X):
        X = np.asarray(X, dtype=float)
        if self.W is None:
            raise ValueError("Model not trained yet; call fit() first.")
        return X.dot(self.W) + self.b

    def predict(self, X):
        return np.where(self.decision_function(X) >= 0.0, 1, -1)


if __name__ == "__main__":
    rng = np.random.default_rng(7)
    X_pos = rng.normal(loc=2.0, scale=0.6, size=(60, 2))
    X_neg = rng.normal(loc=-2.0, scale=0.6, size=(60, 2))
    X = np.vstack([X_pos, X_neg])
    Y = np.hstack([np.ones(len(X_pos)), -np.ones(len(X_neg))])

    mysvm = SVM(lr=0.05, reg=1e-2, epochs=200, batch_size=32, random_state=0, verbose=False)
    w, bias, loss_list = mysvm.fit(X, Y)
    print(w, bias, loss_list[:5], "...")  # show first few losses

    preds = mysvm.predict(X)
    print("Train accuracy:", (preds == Y).mean())
