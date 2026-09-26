"""PyTorch representation of the in-game policy."""
import torch
from torch import nn

INPUTS = 103
HIDDEN = 64
OUTPUTS = 4

class SnakePolicy(nn.Module):
    def __init__(self):
        super().__init__()
        self.fc1 = nn.Linear(INPUTS, HIDDEN)
        self.fc2 = nn.Linear(HIDDEN, OUTPUTS)

    def forward(self, features):
        return self.fc2(torch.relu(self.fc1(features)))


def load_text_weights(model, path):
    with open(path, encoding="ascii") as stream:
        dimensions = tuple(int(v) for v in stream.readline().split())
        if dimensions != (INPUTS, HIDDEN, OUTPUTS):
            raise ValueError(f"unexpected architecture: {dimensions}")
        values = [float(v) for v in stream.read().split()]
    expected = INPUTS*HIDDEN + HIDDEN + HIDDEN*OUTPUTS + OUTPUTS
    if len(values) != expected:
        raise ValueError(f"expected {expected} parameters, found {len(values)}")
    offset = 0
    with torch.no_grad():
        for parameter in (model.fc1.weight, model.fc1.bias,
                          model.fc2.weight, model.fc2.bias):
            size = parameter.numel()
            tensor = torch.tensor(values[offset:offset+size], dtype=torch.float32)
            parameter.copy_(tensor.reshape(parameter.shape))
            offset += size


def export_text_weights(model, path):
    with open(path, "w", encoding="ascii") as stream:
        stream.write(f"{INPUTS} {HIDDEN} {OUTPUTS}\n")
        for parameter in (model.fc1.weight, model.fc1.bias,
                          model.fc2.weight, model.fc2.bias):
            stream.write(" ".join(f"{float(value):.9g}"
                                  for value in parameter.detach().cpu().reshape(-1)))
            stream.write("\n")
