"""Train a small CPU network by imitating the hard path-finding bot.

Generate examples with collect.cpp, then:
    python training/train.py /tmp/snake-training.csv models/neural_weights.txt
Only NumPy is needed during training; the game runs without Python.
"""
import sys
import numpy as np

data = np.loadtxt(sys.argv[1], delimiter=",", dtype=np.float32)
x = data[:, :21]
y = data[:, 21].astype(np.int64)
valid = (y >= 0) & (y < 4)
x, y = x[valid], y[valid]
rng = np.random.default_rng(2026)
order = rng.permutation(len(x))
split = int(0.9 * len(x))
train, test = order[:split], order[split:]
w1 = rng.normal(0, 0.15, (21, 32)).astype(np.float32)
b1 = np.zeros(32, dtype=np.float32)
w2 = rng.normal(0, 0.15, (32, 4)).astype(np.float32)
b2 = np.zeros(4, dtype=np.float32)

for epoch in range(35):
    rng.shuffle(train)
    for ids in np.array_split(train, max(1, len(train) // 256)):
        a = x[ids] @ w1 + b1
        h = np.maximum(a, 0)
        logits = h @ w2 + b2
        logits -= logits.max(axis=1, keepdims=True)
        probabilities = np.exp(logits)
        probabilities /= probabilities.sum(axis=1, keepdims=True)
        probabilities[np.arange(len(ids)), y[ids]] -= 1
        gradient = probabilities / len(ids)
        dw2 = h.T @ gradient
        db2 = gradient.sum(axis=0)
        hidden_gradient = (gradient @ w2.T) * (a > 0)
        dw1 = x[ids].T @ hidden_gradient
        db1 = hidden_gradient.sum(axis=0)
        rate = 0.08
        w2 -= rate * dw2
        b2 -= rate * db2
        w1 -= rate * dw1
        b1 -= rate * db1
    hidden = np.maximum(x[test] @ w1 + b1, 0)
    accuracy = np.mean(np.argmax(hidden @ w2 + b2, axis=1) == y[test])
    if epoch % 5 == 0 or epoch == 34:
        print(f"epoch {epoch+1}: held-out action agreement {accuracy:.3f}")

with open(sys.argv[2], "w", encoding="ascii") as file:
    file.write("21 32 4\n")
    for values in (w1.T.ravel(), b1, w2.T.ravel(), b2):
        file.write(" ".join(f"{float(value):.9g}" for value in values))
        file.write("\n")
print(f"saved {sys.argv[2]} from {len(train)} training states; {len(test)} validation states")
