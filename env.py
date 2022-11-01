import numpy as np


class Environment:
  def __init__(self, ticker_name, depth=10):
    self.ticker_name = ticker_name
    self.ask_book = dict()
    self.bid_book = dict()
    self.has_data = False
    self.depth = depth

  def add_order(self, order):
    type, price, volume, trade = order[0], order[1], order[2], order[3]
    if trade:
      if price in self.ask_book.keys():
        self.ask_book[price] -= volume
        if self.ask_book[price] <= 0:
          del self.ask_book[price]
      elif price in self.bid_book.keys():
        self.bid_book[price] -= volume
        if self.bid_book[price] <= 0:
          del self.bid_book[price]
    else:
      if type == "ask":
        self.ask_book[price] = self.ask_book.get(price, 0) + volume
      if type == "bid":
        self.bid_book[price] = self.bid_book.get(price, 0) + volume
    self.has_data = True

  def get_order_book(self):
    ask_temp = []
    for i in sorted(self.ask_book):
      ask_temp.append((i, self.ask_book[i]))
    ask_temp = ask_temp[:min(self.depth, len(ask_temp))]
    if (len(ask_temp) < self.depth):
      ask_temp += [(0, 0) for i in range(self.depth - len(ask_temp))]
    else:
      ask_temp = ask_temp[:self.depth]

    bid_temp = []
    for i in sorted(self.bid_book, reverse=True):
      bid_temp.append((i, self.bid_book[i]))
    if (len(bid_temp) < self.depth):
      bid_temp += [(0, 0) for i in range(self.depth - len(bid_temp))]
    else:
      bid_temp = bid_temp[:self.depth]

    return bid_temp.reverse() + ask_temp

  def get_order_book_perc(self):
    book = np.array(self.get_order_book())
    total_volume = sum(book)
    return book // total_volume

  def get_current_price(self):
    return (min(self.ask_book) + max(self.bid_book)) // 2
    
