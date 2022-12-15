'''Sample main file'''
from compare_strategy import CompareStrategy
from strategy_analysis import StrategyAnalysis

FILL_PATH1 = "sample_data/BACK_TKSPY_2022-04-02_143804_start_10-30-2019_end_10-30-2019_fill.csv"
ORDER_PATH1 = "sample_data/BACK_TKSPY_2022-04-02_143804_start_10-30-2019_end_10-30-2019_order.csv"
PNL_PATH1 = "sample_data/BACK_TKSPY_2022-04-02_143804_start_10-30-2019_end_10-30-2019_pnl.csv"

# FILL_PATH1 = "data/BACK_Swing10_2022-05-09_172753_start_06-02-2021_end_06-04-2021_fill.csv"
# ORDER_PATH1 = "data/BACK_Swing10_2022-05-09_172753_start_06-02-2021_end_06-04-2021_order.csv"
# PNL_PATH1 = "data/BACK_Swing10_2022-05-09_172753_start_06-02-2021_end_06-04-2021_pnl.csv"

# FILL_PATH1 = "data/BACK_Swing11_2022-05-09_180350_start_06-10-2019_end_06-14-2019_fill.csv"
# ORDER_PATH1 = "data/BACK_Swing11_2022-05-09_180350_start_06-10-2019_end_06-14-2019_order.csv"
# PNL_PATH1 = "data/BACK_Swing11_2022-05-09_180350_start_06-10-2019_end_06-14-2019_pnl.csv"

my_strategy1 = StrategyAnalysis(
    fill_file=FILL_PATH1,
    order_file=ORDER_PATH1,
    pnl_file=PNL_PATH1,
    initial_value=10000000)

FILL_PATH2 = "sample_data/BACK_NONSPY_2022-04-02_143804_start_10-30-2019_end_10-30-2019_fill.csv"
ORDER_PATH2 = "sample_data/BACK_NONSPY_2022-04-02_143804_start_10-30-2019_end_10-30-2019_order.csv"
PNL_PATH2 = "sample_data/BACK_NONSPY_2022-04-02_143804_start_10-30-2019_end_10-30-2019_pnl.csv"

my_strategy2 = StrategyAnalysis(
    fill_file=FILL_PATH2,
    order_file=ORDER_PATH2,
    pnl_file=PNL_PATH2,
    initial_value=10000000)



my_strategy1.visualize_pnl()
my_strategy2.visualize_pnl()

cs = CompareStrategy()
cs.add_strategy(my_strategy1)
cs.add_strategy(my_strategy2)

cs.visualize_strategies()
cs.measurement_table()
