"""Train with PyTorch; save a .pth checkpoint and export weights for C++.

Run from the project root:
  py training/train_torch.py --init-text models/neural_weights.txt \
      --checkpoint models/neural_checkpoint.pth \
      --export models/neural_candidate.txt teacher.csv dagger.csv
"""
import argparse
from pathlib import Path
import numpy as np
import torch
from torch import nn
from torch.utils.data import DataLoader, TensorDataset
from torch_model import SnakePolicy, INPUTS, HIDDEN, OUTPUTS, load_text_weights, export_text_weights

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("samples", nargs="+", help="CSV files from collect.cpp")
    parser.add_argument("--checkpoint", required=True, help="PyTorch checkpoint (.pth)")
    parser.add_argument("--export", required=True, help="C++ weights text output")
    parser.add_argument("--init-text", help="Initialize from existing C++ weights")
    parser.add_argument("--resume", help="Continue training from --checkpoint")
    parser.add_argument("--epochs", type=int, default=12)
    parser.add_argument("--batch-size", type=int, default=256)
    args = parser.parse_args()
    if args.epochs < 1 or args.batch_size < 1:
        parser.error("epochs and batch size must be positive")
    if args.resume and args.init_text:
        parser.error("choose either --resume or --init-text")

    torch.manual_seed(2026)
    torch.set_num_threads(min(4, torch.get_num_threads()))
    samples = np.concatenate(
        [np.loadtxt(path, delimiter=",", dtype=np.float32) for path in args.samples],
        axis=0,
    )
    if samples.ndim != 2 or samples.shape[1] != INPUTS + 1:
        raise ValueError(f"expected {INPUTS + 1} CSV columns")
    labels = samples[:, INPUTS].astype(np.int64)
    valid = (labels >= 0) & (labels < OUTPUTS)
    features, labels = samples[valid, :INPUTS], labels[valid]
    order = np.random.default_rng(2026).permutation(len(features))
    split = int(len(order) * 0.9)
    if split == 0 or split == len(order):
        raise ValueError("not enough training examples")
    train_ids, val_ids = order[:split], order[split:]
    model = SnakePolicy()
    if args.init_text:
        load_text_weights(model, args.init_text)
    optimizer = torch.optim.Adam(model.parameters(), lr=0.0003)
    start_epoch = 0
    if args.resume:
        state = torch.load(args.checkpoint, map_location="cpu", weights_only=True)
        if state.get("architecture") != [INPUTS, HIDDEN, OUTPUTS]:
            raise ValueError("checkpoint architecture mismatch")
        model.load_state_dict(state["model"])
        optimizer.load_state_dict(state["optimizer"])
        start_epoch = int(state["epoch"])

    dataset = TensorDataset(torch.from_numpy(features[train_ids]),
                            torch.from_numpy(labels[train_ids]))
    loader = DataLoader(dataset, batch_size=args.batch_size, shuffle=True)
    validation_x = torch.from_numpy(features[val_ids])
    validation_y = torch.from_numpy(labels[val_ids])
    for epoch in range(args.epochs):
        model.train()
        for x, y in loader:
            optimizer.zero_grad()
            loss = nn.functional.cross_entropy(model(x), y)
            loss.backward()
            optimizer.step()
        model.eval()
        with torch.no_grad():
            accuracy = (model(validation_x).argmax(1) == validation_y).float().mean().item()
        print(f"epoch {start_epoch + epoch + 1}: validation agreement {accuracy:.3f}", flush=True)

    Path(args.checkpoint).parent.mkdir(parents=True, exist_ok=True)
    Path(args.export).parent.mkdir(parents=True, exist_ok=True)
    torch.save({
        "architecture": [INPUTS, HIDDEN, OUTPUTS],
        "model": model.state_dict(),
        "optimizer": optimizer.state_dict(),
        "epoch": start_epoch + args.epochs,
    }, args.checkpoint)
    export_text_weights(model, args.export)

    # Check that the C++ file reproduces PyTorch's output after text rounding.
    mirror = SnakePolicy()
    load_text_weights(mirror, args.export)
    probe = torch.from_numpy(features[val_ids[:min(128, len(val_ids))]])
    with torch.no_grad():
        difference = (model(probe) - mirror(probe)).abs().max().item()
    if difference > 1e-5:
        raise RuntimeError(f"C++ export changed logits: max difference {difference}")
    print(f"saved {args.checkpoint} and {args.export}; export error {difference:.2g}")


if __name__ == "__main__":
    main()
