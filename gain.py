import numpy as np
import pandas as pd
from collections import Counter
from sklearn.model_selection import train_test_split

class DecisionTreeID3:
    def __init__(self, max_depth=None, min_samples_split=2):
        self.tree = None
        self.target_name = None
        self.max_depth = max_depth
        self.min_samples_split = min_samples_split
        self.majority_class_ = None

    # ----- impurity & gain -----
    def entropy(self, y: pd.Series) -> float:
        n = len(y)
        if n == 0:
            return 0.0
        _, counts = np.unique(y, return_counts=True)
        p = counts / n
        # compute only for p>0 to avoid log2(0)
        p = p[p > 0]
        return -np.sum(p * np.log2(p))

    def information_gain(self, data: pd.DataFrame, feature_name: str, target_name: str) -> float:
        total_entropy = self.entropy(data[target_name])
        values, counts = np.unique(data[feature_name], return_counts=True)
        n = len(data)
        weighted = 0.0
        for v, c in zip(values, counts):
            subset = data[data[feature_name] == v]
            weighted += (c / n) * self.entropy(subset[target_name])
        return total_entropy - weighted

    # ----- tree building -----
    def build_tree(self, data: pd.DataFrame, features: list, target_name: str, depth: int = 0):
        # pure leaf
        uniq = np.unique(data[target_name])
        if len(uniq) == 1:
            return uniq[0]

        # stopping criteria
        if not features or len(data) < self.min_samples_split or (
            self.max_depth is not None and depth >= self.max_depth
        ):
            return Counter(data[target_name]).most_common(1)[0][0]

        # choose best feature
        gains = {f: self.information_gain(data, f, target_name) for f in features}
        best_feature = max(gains, key=gains.get)

        tree = {best_feature: {}}
        remaining = [f for f in features if f != best_feature]

        # split on each category of best_feature
        for value in np.unique(data[best_feature]):
            subset = data[data[best_feature] == value]
            if subset.empty:
                tree[best_feature][value] = Counter(data[target_name]).most_common(1)[0][0]
            else:
                tree[best_feature][value] = self.build_tree(subset, remaining, target_name, depth + 1)
        return tree

    # ----- public API -----
    def fit(self, data: pd.DataFrame, target_name: str = None):
        # infer common target names if not given
        if target_name is None:
            for cand in ("play", "Play", "target", "label", "y"):
                if cand in data.columns:
                    target_name = cand
                    break
            if target_name is None:
                raise ValueError("Please provide target_name (e.g., 'Play' or 'play').")
        self.target_name = target_name

        # features (ignore obvious ID column if present)
        features = [c for c in data.columns if c != target_name and c.lower() != "day"]
        if not features:
            raise ValueError("No features found to train on.")

        self.majority_class_ = Counter(data[target_name]).most_common(1)[0][0]
        self.tree = self.build_tree(data, features, target_name)
        return self

    def _predict_one(self, x: pd.Series):
        node = self.tree
        while isinstance(node, dict):
            feature = next(iter(node))
            branches = node[feature]
            val = x.get(feature)

            if val in branches:
                node = branches[val]
            else:
                # unseen category: pick majority among child leaves
                def collect_leaves(n, bag: Counter):
                    if isinstance(n, dict):
                        for v in n.values():
                            collect_leaves(v, bag)
                    else:
                        bag[n] += 1
                counts = Counter()
                for child in branches.values():
                    collect_leaves(child, counts)
                if counts:
                    return counts.most_common(1)[0][0]
                return self.majority_class_
        return node

    def predict(self, X: pd.DataFrame):
        return np.array([self._predict_one(row) for _, row in X.iterrows()])

    def score(self, X: pd.DataFrame, y: pd.Series):
        preds = self.predict(X)
        return (preds == np.array(y)).mean()


def main():
    # Example usage with the classic Play Tennis dataset
    df = pd.read_csv("play_tennis.csv")
    target_name = "play" if "play" in df.columns else ("Play" if "Play" in df.columns else None)
    if target_name is None:
        raise ValueError("Target column not found. Expected 'play' or 'Play'.")

    # drop rows missing target
    df = df.dropna(subset=[target_name])

    X = df.drop(columns=[target_name, "day"], errors="ignore")
    y = df[target_name]

    # 90/10 split, stratified
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=0.1, random_state=42, stratify=y
    )

    dt = DecisionTreeID3(max_depth=None, min_samples_split=2)
    dt.fit(pd.concat([X_train, y_train], axis=1), target_name=target_name)
    acc = dt.score(X_test, y_test)

    print("Learned tree:")
    print(dt.tree)
    print(f"\nTest accuracy: {acc:.3f}")

if __name__ == "__main__":
    main()

