import itertools
import env
import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

action = ["sell", "stay", "buy"]
position = ["neutral", "long", "short"]
NUM_POSITION = 3
NUM_ACTION = 3
act_pos_pair = list(itertools.product(position, action))


class Agent:
  def __init__(self, name, depth, lrate):
    hidden_layer = 10
    self.hidden = nn.Linear(depth, hidden_layer)
    self.output = nn.Linear(hidden_layer, 3)
    self.env = env.Environment(name)
    self.fn = nn.Sigmoid()
    self.lrate = lrate
    self.criterion = nn.CrossEntropyLoss()
    self.optimizer = optim.SGD(self.parameters(), lr=self.lrate)

    self.exploration_rate = 1
    self.exploration_rate_decay = 0.99999975
    self.exploration_rate_min = 0.1

  def act(self, environment):
    if np.random.rand() < self.exploration_rate:  # EXPLORE
      action_idx = np.random.randint(NUM_ACTION)
    else:  # EXPLOIT
      action_values = self.forward(environment.get_order_book_perc())
      action_idx = torch.argmax(action_values).item()

    # decrease exploration_rate
    self.exploration_rate *= self.exploration_rate_decay
    self.exploration_rate = max(
        self.exploration_rate_min, self.exploration_rate)

    return action_idx

  def forward(self, x):
    x = self.fn(self.hidden(x))
    out = self.output(x)
    return self.fn(out)

  def step(self, x, environment):
    threshold = 10
    diff = threshold * (environment.get_current_price() -
                        self.env.get_current_price())
    y = np.sign(int(diff)) + 1 
    # 0: curr price less than prev price, should sell
    # 1: curr price same as prev price, should stay and wait
    # 2: curr price higher than prev price, should buy
    self.optimizer.zero_grad()
    loss = self.criterion(self.forward(x), y)
    loss.backward()
    self.optimizer.step()
    return loss.detach().cpu().numpy()
    
