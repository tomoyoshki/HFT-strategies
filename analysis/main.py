'''
Analysis runner
'''

import os
import sys
import glob

import pandas as pd
from compare_strategy import CompareStrategy
from strategy_analysis import StrategyAnalysis


my_strategies = CompareStrategy()
TICKER_DIRECTORY = "/home/vagrant/Desktop/strategy_studio/backtesting/text_tick_data"
DATA_DIRECTORY = "/home/vagrant/Desktop/strategy_studio/backtesting/backtesting-cra-exports"


def parse_files(strategy_name, strategy_id = None):
    '''
    Parse input files
    '''
    strategy_name = f"_{strategy_name}_"
    result_files = []

    for file in os.listdir(DATA_DIRECTORY):
        if strategy_name in file:
            result_files.append(DATA_DIRECTORY + "/" + file)

    if len(result_files) < 3:
        print("Invalid file, please re-enter!\n")
        return None

    if len(result_files) > 3:
        if strategy_id is None:
            strategy_id = input(
                "Please enter the id of your strategy\n"
            )
        new_result_files = []
        for file in result_files:
            if strategy_id in file:
                new_result_files.append(file)
        result_files = new_result_files
        if len(result_files) < 3:
            print("Invalid file, please re-enter!\n")
            return None

    return_files = ["", "", ""]
    for target_file in result_files:
        if target_file[-8:] == "fill.csv":
            return_files[0] = target_file
        elif target_file[-9:] == "order.csv":
            return_files[1] = target_file
        elif target_file[-7:] == "pnl.csv":
            return_files[2] = target_file
    return return_files

def parse_ticks(tick_name):
    '''
    Parse ticker files
    '''
    tick_name = f"_{tick_name}_"
    tick_files = []
    for file in os.listdir(TICKER_DIRECTORY):
        if tick_name in file:
            tick_files.append(TICKER_DIRECTORY + "/" + file)
    return tick_files

def process_interactive():
    '''
    Interative mode for users to add strategy
    '''

    print("Parsing Strategy studio export files")
    print("="*30)
    while True:
        strategy_name = input(
            "Please enter the name of your strategy\n"
        )
        result_files = parse_files(strategy_name)
        if result_files is None:
            continue
        tick_name = input(
            "Please enter the name of the tickers\n"
        )
        tick_files = parse_ticks(tick_name)
        if tick_files is None:
            print("Invalid Tick files")
        strategy = StrategyAnalysis(
                    fill_file=result_files[0],
                    order_file=result_files[1],
                    pnl_file=result_files[2],
                    initial_value=10000000)
        if len(tick_files) > 0:
            strategy.read_ticks(tick_files)
        my_strategies.add_strategy(strategy)
        if len(tick_files) > 0:
            strategy.visualize_pnl()
        end = input(
            "Do you want to continue adding strategy? (yes / no)\n"
            ).strip().lower()
        if end in ('no', 'n'):
            break
    if len(tick_files) > 0:
        my_strategies.visualize_strategies()
    my_strategies.measurement_table()

def process_cml(fill_, order_, pnl_, strategy_tick_):
    '''
    Process Command Line Arguments
    '''
    tick_files = parse_ticks(strategy_tick_)
    strategy = StrategyAnalysis(
                fill_file=fill_,
                order_file=order_,
                pnl_file=pnl_,
                initial_value=10000000)
    if len(tick_files) > 0:
        strategy.read_ticks(tick_files)
    my_strategies.add_strategy(strategy)
    if len(tick_files) > 0:
        strategy.visualize_pnl()
    my_strategies.measurement_table()

if __name__ == "__main__":

    if not os.path.exists(DATA_DIRECTORY):
        print(f"Export directory not found: {DATA_DIRECTORY}")
        sys.exit()
    if not os.path.exists(TICKER_DIRECTORY):
        print(f"Tick data directory not found: {TICKER_DIRECTORY}")
    list_of_files = glob.glob(f"{DATA_DIRECTORY}/*")
    if len(list_of_files) == 0:
        print("No such file found")
        sys.exit()
    list_of_files.sort(key=os.path.getctime)
    FILL = ""
    ORDER = ""
    PNL = ""
    for new_file in list_of_files[-3:]:
        print(new_file)
        if new_file[-8:] == "fill.csv":
            FILL = new_file
        elif new_file[-9:] == "order.csv":
            ORDER = new_file
        elif new_file[-7:] == "pnl.csv":
            PNL = new_file
    print(FILL)
    print(ORDER)
    print(PNL)
    fill_df = pd.read_csv(FILL)
    strategy_tick = fill_df["Symbol"][0]
    if not os.path.exists("figs/"):
        os.mkdir("figs")
    if len(sys.argv) == 1:
        process_cml(FILL, ORDER, PNL, strategy_tick)
    elif sys.argv[2] == "-i":
        process_interactive()
