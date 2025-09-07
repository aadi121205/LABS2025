import numpy as np
import pandas as pd
from collections import Counter
from sklearn.model_selection import train_test_split

class DecisionTreeGini:
    def __init__(self, max_depth=None, min_samples_split=2):
        self.tree = None
        self.target_name = None
        self.max_depth = max_depth
        self.min_samples_split = min_samples_split
        self.majority_class_ = None  # for safety at inference

    # ----- impurity & gain -----
    def gini_impurity(self, y: pd.Series) -> float:
        if len(y) == 0:
            return 0.0
        classes, counts = np.unique(y, return_counts=True)
        p = counts / counts.sum()
        return 1.0 - np.sum(p ** 2)

    def gini_gain(self, data: pd.DataFrame, feature_name: str, target_name: str) -> float:
        total_gini = self.gini_impurity(data[target_name])
        values, counts = np.unique(data[feature_name], return_counts=True)
        weighted = 0.0
        n = len(data)
        for v, c in zip(values, counts):
            subset = data[data[feature_name] == v]
            weighted += (c / n) * self.gini_impurity(subset[target_name])
        return total_gini - weighted

    # ----- tree building -----
    def build_tree(self, data: pd.DataFrame, features: list, target_name: str, depth: int = 0):
        # if all same class -> leaf
        uniq = np.unique(data[target_name])
        if len(uniq) == 1:
            return uniq[0]

        # stopping criteria
        if not features or len(data) < self.min_samples_split or (
            self.max_depth is not None and depth >= self.max_depth
        ):
            return Counter(data[target_name]).most_common(1)[0][0]

        # choose best feature
        gains = {f: self.gini_gain(data, f, target_name) for f in features}
        best_feature = max(gains, key=gains.get)
        tree = {best_feature: {}}
        remaining = [f for f in features if f != best_feature]

        # split on categories of best_feature
        for value in np.unique(data[best_feature]):
            subset = data[data[best_feature] == value]
            if subset.empty:
                tree[best_feature][value] = Counter(data[target_name]).most_common(1)[0][0]
            else:
                tree[best_feature][value] = self.build_tree(subset, remaining, target_name, depth + 1)
        return tree

    # ----- public API -----
    def fit(self, data: pd.DataFrame, target_name: str = None):
        # infer target name if not given
        if target_name is None:
            if "play" in data.columns:
                target_name = "play"
            elif "Play" in data.columns:
                target_name = "Play"
            else:
                raise ValueError("Please provide target_name; could not find 'play' or 'Play' column.")
        self.target_name = target_name

        # drop obvious ID columns if present
        features = [c for c in data.columns if c != target_name and c.lower() != "day"]

        # remember training majority class as fallback
        self.majority_class_ = Counter(data[target_name]).most_common(1)[0][0]

        self.tree = self.build_tree(data, features, target_name)
        return self

    def _predict_one(self, x: pd.Series):
        node = self.tree
        while isinstance(node, dict):
            feature = next(iter(node))
            branches = node[feature]
            value = x.get(feature)

            # follow known branch; otherwise fallback to majority in branches
            if value in branches:
                node = branches[value]
            else:
                # unseen category at inference time
                # choose the most common class among child leaves
                leaf_counts = Counter()
                for child in branches.values():
                    if isinstance(child, dict):
                        # flatten one level deeper to find leaves
                        sub_counts = Counter()
                        def collect_leaves(n):
                            if isinstance(n, dict):
                                for v in n.values():
                                    collect_leaves(v)
                            else:
                                sub_counts[n] += 1
                        collect_leaves(child)
                        leaf_counts += sub_counts
                    else:
                        leaf_counts[child] += 1
                if leaf_counts:
                    return leaf_counts.most_common(1)[0][0]
                return self.majority_class_
        return node

    def predict(self, X: pd.DataFrame):
        return np.array([self._predict_one(row) for _, row in X.iterrows()])

    def score(self, X: pd.DataFrame, y: pd.Series):
        preds = self.predict(X)
        return (preds == np.array(y)).mean()


def main():
    # Load dataset (works with headers like: day,outlook,temp,humidity,wind,play)
    df = pd.read_csv("play_tennis.csv")
    # Auto-detect target (play/Play) or set explicitly: target_name="play"
    target_name = "play" if "play" in df.columns else ("Play" if "Play" in df.columns else None)
    if target_name is None:
        raise ValueError("Target column not found. Expected 'play' or 'Play'.")

    # Basic cleanup: drop rows with missing target
    df = df.dropna(subset=[target_name])

    # Train/test split 90/10 with stratification
    X = df.drop(columns=[target_name, "day"], errors="ignore")
    y = df[target_name]
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=0.1, random_state=42, stratify=y
    )

    # Fit and evaluate
    dt = DecisionTreeGini(max_depth=None, min_samples_split=2)
    dt.fit(pd.concat([X_train, y_train], axis=1), target_name=target_name)
    acc = dt.score(X_test, y_test)

    print("Learned tree:")
    print(dt.tree)
    print(f"\nTest accuracy: {acc:.3f}")

if __name__ == "__main__":
    main()
