"""Create a PyTorch checkpoint from the currently deployed C++ weights."""
import argparse
import torch
from torch_model import SnakePolicy, INPUTS, HIDDEN, OUTPUTS, load_text_weights

parser = argparse.ArgumentParser()
parser.add_argument("weights")
parser.add_argument("checkpoint")
args = parser.parse_args()

model = SnakePolicy()
load_text_weights(model,args.weights)
optimizer = torch.optim.Adam(model.parameters(),lr=0.0003)
torch.save({"architecture":[INPUTS,HIDDEN,OUTPUTS],
            "model":model.state_dict(),
            "optimizer":optimizer.state_dict(),
            "epoch":0}, args.checkpoint)
print(f"wrote {args.checkpoint}")
