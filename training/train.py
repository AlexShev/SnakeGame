"""Train a 103 -> 64 -> 4 CPU policy from teacher and DAgger samples.

Usage: python training/train.py output.txt teacher.csv [student_states.csv ...]
"""
import sys
import numpy as np

if len(sys.argv) < 3:
    raise SystemExit("usage: train.py output.txt samples.csv [more.csv ...]")
output, files = sys.argv[1], sys.argv[2:]
data = np.concatenate([np.loadtxt(path, delimiter=",", dtype=np.float32)
                       for path in files], axis=0)
x = data[:, :103]
y = data[:, 103].astype(np.int64)
valid = (y >= 0) & (y < 4)
x, y = x[valid], y[valid]
rng = np.random.default_rng(2026)
order = rng.permutation(len(x))
split = int(0.9 * len(x))
train, test = order[:split], order[split:]
w1 = rng.normal(0, np.sqrt(2/103), (103,64)).astype(np.float32)
b1 = np.zeros(64, dtype=np.float32)
w2 = rng.normal(0, np.sqrt(2/64), (64,4)).astype(np.float32)
b2 = np.zeros(4, dtype=np.float32)
params = [w1,b1,w2,b2]
momentum = [np.zeros_like(p) for p in params]
variance = [np.zeros_like(p) for p in params]
step = 0

for epoch in range(24):
    rng.shuffle(train)
    for ids in np.array_split(train, max(1, len(train)//256)):
        a = x[ids] @ w1 + b1
        h = np.maximum(a,0)
        logits = h @ w2 + b2
        logits -= logits.max(axis=1,keepdims=True)
        probs = np.exp(logits)
        probs /= probs.sum(axis=1,keepdims=True)
        probs[np.arange(len(ids)),y[ids]] -= 1
        gradient = probs/len(ids)
        dw2 = h.T @ gradient
        db2 = gradient.sum(axis=0)
        dh = (gradient @ w2.T)*(a>0)
        dw1 = x[ids].T @ dh
        db1 = dh.sum(axis=0)
        step += 1
        for p,g,m,v in zip(params,[dw1,db1,dw2,db2],momentum,variance):
            m *= 0.9
            m += 0.1*g
            v *= 0.999
            v += 0.001*(g*g)
            p -= 0.001*(m/(1-0.9**step))/(np.sqrt(v/(1-0.999**step))+1e-8)
    if epoch%5==0 or epoch==23:
        h = np.maximum(x[test] @ w1+b1,0)
        accuracy = np.mean(np.argmax(h @ w2+b2,axis=1)==y[test])
        print(f"epoch {epoch+1}: held-out action agreement {accuracy:.3f}",flush=True)

with open(output,"w",encoding="ascii") as file:
    file.write("103 64 4\n")
    for values in (w1.T.ravel(),b1,w2.T.ravel(),b2):
        file.write(" ".join(f"{float(v):.9g}" for v in values)+"\n")
print(f"saved {output}: {len(train)} training, {len(test)} validation states")
