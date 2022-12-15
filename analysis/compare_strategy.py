'''
Python Class that compares the two or more strategies
'''
from random import sample
import numpy as np
import pandas as pd
import plotly.graph_objects as go

class CompareStrategy():
    """
    A class compare strategies.
    ...

    Attributes
    ----------
    strategy_dict : dict(str, StrategyAnalysis)
        Strategy container to compare each analysis for comparison

    Methods
    -------
    add_strategy(strategy)
        add the given strategy to our container

    get_strategy(name=None)
        return the desired strategy
    """
    def __init__(self):
        self.strategy_dict = {}

    # Add a strategy to the dictionary
    def add_strategy(self, strategy):
        '''Add the StrategyAnalysis object to our container'''
        if strategy in self.strategy_dict:
            print("Warning: Strategy already exist, overwriting")
        self.strategy_dict[strategy.name] = strategy

    # Return the user specified strategy
    def get_strategy(self, name=None):
        '''Return the StrategyAnalysis object with the given name'''
        if name is None or not name in self.strategy_dict:
            return self.strategy_dict
        return self.strategy_dict

    # get Strategy in Dataframe Formate
    def get_strategy_df(self):
        '''Return the data frame containing all the strategies'''
        strategy_columns = []
        for key, value_strategy in self.strategy_dict.items():
            strategy_pnl = value_strategy.get_data()['Cumulative PnL'].to_list()
            strategy_columns.append([key] + strategy_pnl)

        strategy_columns = np.array(strategy_columns).T
        try:
            strategy_df = pd.DataFrame(strategy_columns[1:], columns=strategy_columns[0])
        except Exception as exc:
            raise Exception("Invalid size") from exc

        key = list(self.strategy_dict)[0]
        strategy_df.index = self.strategy_dict[key].get_data()['Time'].to_list()
        strategy_df.index.name = "Date"
        strategy_df.columns.name = "Company"
        return strategy_df

    # Visualize the strategies
    def visualize_strategies(self):
        '''Visualize all the strategies'''
        color_map = [
            ("green", "red"),
            ("#4c78a8", "#BAB0AC"),
            ("#636efa", "#fecb52"),
            ("#00fe35", "#fc6955")
        ]
        date_ = None
        fig = go.Figure()
        counter = 1
        for key in list(self.strategy_dict.keys()):
            date_ = self.strategy_dict[key].date_label
            strategy_candle_df =  pd.DataFrame(
                {
                        "open":[arr[0] for arr in self.strategy_dict[key].pnl_by_date],
                        "high":[max(arr) for arr in self.strategy_dict[key].pnl_by_date],
                        "low":[min(arr) for arr in self.strategy_dict[key].pnl_by_date],
                        "close":[arr[-1] for arr in self.strategy_dict[key].pnl_by_date],
                }
            )
            random_color = sample(color_map, 1)[0]
            fig.add_trace(
                go.Candlestick(
                    x=date_,
                    open=strategy_candle_df["open"],
                    high=strategy_candle_df["high"],
                    low=strategy_candle_df["low"],
                    close=strategy_candle_df["close"],
                    name=self.strategy_dict[key].name,
                    yaxis=f"y{counter}",
                    increasing_line_color=random_color[0],
                    decreasing_line_color=random_color[1]
                )
            )
            counter += 1
        fig.update_layout(yaxis2=dict(overlaying='y1', side='right'))
        # fig.show()

    # Get the Measurements for each strategy as a Dataframe
    def get_measurements(self):
        '''Return the data frame of strategies with its evaluation metrics'''
        dict_key = list(self.strategy_dict.keys())
        strategy_columns = []
        for key in dict_key:
            strategy_measurements = self.strategy_dict[key].measure_strategy()
            strategy_columns.append([key] + strategy_measurements)
        measurement_columns = [
            "Strategy Names",
            "Initial Value",
            "Final Value",
            "Begin Time",
            "End Time",
            "Net PnL",
            "Max PnL",
            "Min PnL",
            "Cumulative Returns",
            "Sharpe Ratio",
            "Max DrownDown"]
        strategy_df = pd.DataFrame(strategy_columns, columns= measurement_columns)
        return strategy_df

    # Output the Table with measurement for each strategy
    def measurement_table(self):
        """Output table represent the evaluation metrics for all strategies"""
        measurement_df = self.get_measurements()
        row_names = measurement_df.columns.to_numpy()[1:].reshape((10, 1)).T
        row_values = measurement_df.to_numpy()[:, 1:]
        rows = np.concatenate((row_names, row_values), axis=0)
        strategy_name_list =  measurement_df["Strategy Names"].to_list()
        names_ = "_".join(strategy_name_list)
        fig = go.Figure(
            data=[
                go.Table(header=dict(values=["Types"] + strategy_name_list),
                cells=dict(values=rows))
            ])
        # fig.show()
        fig.write_image(f"figs/{names_}_measurement.png")
