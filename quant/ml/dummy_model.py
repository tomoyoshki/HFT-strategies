import torch
import torchvision

# This is a dummy pytorch model;
# Run this file to save the model for C++ to load.
# https://pytorch.org/tutorials/advanced/cpp_export.html

class DummyModel(torch.nn.Module):
    def __init__(self, N, M):
        super(DummyModel, self).__init__()
        self.weight = torch.nn.Parameter(torch.rand(N, M))
        self.last_buy = True
        self.SIZE = 100
        self.BUY = 1
        self.SELL = 0
    def forward(self, input):
        if self.last_buy:
          self.last_buy = False
          return torch.as_tensor([self.SELL, self.SIZE])
        else:
          self.last_buy = True
          return torch.as_tensor([self.BUY, self.SIZE])

my_module = DummyModel(10,20)
sm = torch.jit.script(my_module)
sm.save("saved_dummy_model.pt")