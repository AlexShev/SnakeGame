"""Export a PyTorch checkpoint into a LibTorch-loadable module.

The checkpoint (.pth) is for continued training. The TorchScript .pt is for
C++ inference. TorchScript is deprecated upstream but remains a supported
LibTorch interchange path for this small static model.
"""
import argparse
import torch
from torch_model import SnakePolicy, INPUTS, HIDDEN, OUTPUTS

parser = argparse.ArgumentParser()
parser.add_argument("checkpoint")
parser.add_argument("output")
args = parser.parse_args()

checkpoint = torch.load(args.checkpoint, map_location="cpu", weights_only=True)
if checkpoint.get("architecture") != [INPUTS, HIDDEN, OUTPUTS]:
    raise ValueError("checkpoint architecture mismatch")
model = SnakePolicy()
model.load_state_dict(checkpoint["model"])
model.eval()
example = torch.zeros(1, INPUTS)
scripted = torch.jit.trace(model, example)
scripted.save(args.output)
reloaded = torch.jit.load(args.output, map_location="cpu").eval()
probe = torch.randn(32, INPUTS)
with torch.no_grad():
    difference = (model(probe) - reloaded(probe)).abs().max().item()
if difference > 1e-5:
    raise RuntimeError(f"export changed logits: {difference}")
print(f"saved {args.output}; maximum logit difference {difference:.2g}")
