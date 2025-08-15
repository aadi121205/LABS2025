import numpy as np

data = np.loadtxt('housing.csv', delimiter=',')

X = data[:, 0:-1]
y = data[:, -1]

print(X.shape)
print(y.shape)

XB = np.c_[np.ones((X.shape[0], 1)), X]


theta = np.linalg.inv(XB.T @ XB) @ XB.T @ y

print("Coefficients (theta):", theta)

for i in range(5):
    sample_X = XB[i, :]
    prediction = sample_X @ theta
    print(f"Sample {i}: Actual = {y[i]:.2f}, Predicted = {prediction:.2f}")
