# High Frequency Trading Strategies with RCM Strategy Studio

[!toc]

**Group 1 FIN 556 High Frequency Trading Final Report**

## People

**Ruipeng (Ray), Han**
- TODO

**Yihong, Jian**

- Yihong is a senior studying computer science at University of Illinois, Urbana-Champaign graduating in May 2023. He is passionate about lower level programmings with experince in multiple fields such as networks, distributed systems, and kernels. He has multiple project experiences with C++, such as writing a HDFS from scratch. 

**Tomoyoshi (Tommy), Kimura**

- TODO

**Kaiyuan, Luo**
- TODO


## Introduction

### Project Description

This is a semester-long project for the [FIN 556](https://courses.illinois.edu/schedule/2022/fall/FIN/556) – "Algorithmic Market Microstructure", instructed by Professor [David Lariviere](https://davidl.web.illinois.edu/).

Our project aims to: 

- Develop alogrithmic trading strategies using nano-second accurate data, and
- Generate figures and anlaysize our strategy performance.

We further break down our project into four sections: data retrival and parsing, strategy development, and analysis

1. Data Retrival and Parsing:

   The main market data sources for this project is IEX. We adopted professor Lariviere's IEX data downloader and parser for the project.

2. Strategy Development:

   We use Strategy Studio as the baseground to develop and backtest trading strategies. Strategy Studio is a proprietary software from RCM used for trading strategy developement and testing. Inside Strategy Studio, we create new trading strategies by implementing its interface (for example, inheirting the `Strategy` class). Then, we backtest our strategy in Strategy Studio using the data from the Data Retrival and Parsing section. The backtesting process will create three file (orders, fills, and profit-and-loss) which we can use to evaluate the strategy's performance.

   We focused on developing two strategies. Kalman Filter strategy and Reinforcement Learning based Strategy.

3. Analysis:

   For the analysis part, we implemented interpretation and visualization script for SS backtesting output. We utilized several powerful packages to generate interactive plots for both analysis and comparison. 

### Technologies

#### Programming Languages
- Bash
  - A scripting language that allow users to chain series of commands and feed into computer
  - We write bash scripts for automation works.
- C++
  - Strategy Studio provides the interface entirely in C++ and allow us to implement various strategies in C++. 
- Python
  - We used Python for several tasks dealing with data.
  - We used Python to train deep learning models for ML based strategies.
  - Python is also the major language for result analysis and visualization. We chose Python because there is a very powerful Visualization for financial market data called Plotly. 


#### Softwares

- Strategy Studio
  - We are grateful that **RCM** has sponsored us to give us free access to Strategy Studio for implementing and backtesting our strategies with the market data.

- LibTorch
  - The C++ SDK for PyTorch. We trained models in python and export as a model file. Then we read model file in the C++ strategy code and run predictions.

- Eigen
  - Eigen is an open source C++ linear algebra library that supports the header only KalmanFilter library. Luckly Strategy Studio ships with it so we don't have to manually install it.

- Jupyter Notebook
  - Jupyter Notebook is light weight dev tool for Python and it's commonly used in datascience. We used the tool to train ML models.

#### Development Frameworks

- Virtual Box/Vagrant
  - Virtualbox is a software that is used to create virtual machines
  - Vagrant can use virtualbox to launch standardrized VMs to keep consistency in each run of our project.
  - We used vagrant and virtualbox to set up the environment that runs our project.
  - Our vagrant box contained Software Studio and necessary enviroment. It derived from Prof's Fin566 box.

- Google Colab
  - Colab is a platform hosted by Google
  - We trained models on the platform

- VS Code
  - The de facto text editor that everyone uses
  - We use VS Code to SSH into vagrant to do devs (including writing this report)

#### Opearting Systems
- Ubuntu 20.04

#### Packages

- Strategy Studios Includes

  Strategy Studios contains libraries defined in its software for developing strategies. Please see the stratege studio specifications for detailed documentaions on libraries and interfaces.

- Analysis packages

  ```bash
  plotly
  numpy
  pandas
  kaleido
  ```

**Repository Layout**

```bash
group_01_project
└── analysis
└── docs
└── quant
└── strategies
  ├── kalman_filter_strategy
	  ├── KalmanFilterStrategy.cpp
	  ├── KalmanFilterStrategy.h
    ├── Makfile
    ├── kalman_filter.cpp
    ├── kalman_filter.h

```

## Usage:

To run our compiled demo, follow these steps:
1. Download and install virtual box and vagrant
2. Clone our repo:
```bash
> git clone https://gitlab.engr.illinois.edu/fin556_algo_market_micro_fall_2022/fin556_algo_fall_2022_group_01/group_01_project.git
```
3. Copy strategy into SS sdk
4. Compile
5. Copy ```.so``` file to bt directory
6. Run bt 


### IEX Data parsers

We directly imported the Professor's IEX downloader/parser as a submodule of our project so we can directly use it to download DEEP and TRADE data from IEX exchange that is ready to be feed into Strategy Studio for strategy development and backtesting. The link to Professor's IEX downloader/parser is [here](https://gitlab.engr.illinois.edu/shared_code/iexdownloaderparser). Details on directions of using the IEX downloader/parser can be found on the README.md under the project root directory.

```bash
parser
└── iex_parser
	├── src
	    ├── download_iex_pcaps.py
	    ├── parse_compress_iex_pcaps.py
    	├── parse_iex_caps.py
    	├── stockbook.py
    ├── README.md
    ├── download.sh
    ├── parse_all.sh
    
```

1. Direct to the IexDownloaderParser directory `cd parser/IexDownloaderParser`and run <code>./download.sh</code> to download the source IEX deep data (.gz format). To retrieve data in a specific range of dates, open and edit the download.sh, only modifies the start-date and end-date arguments:

   ```bash
   python3 src/download_iex_pcaps.py --start-date 2022-01-01 --end-date 2022-06-01 --download-dir data/iex_downloads
   ```

   Note that git-submodules need to be pulled separately, detailed instruction for pulling git-submodule can be found [here](https://stackoverflow.com/questions/1030169/easy-way-to-pull-latest-of-all-git-submodules)

2. Check that the downloaded raw IEX DEEP dat files should be stored at `iexdownloaderparsers/data/iex_downlaods/DEEP`

3. Run `./parse_all.sh` to parse IEX deep data. Result will be stored under `iexdownloaderparsers/data/text_tick_data` with the foramt `tick_SYMBOL_YYYYMMDD.txt.gz`.  

   To specify the company symbols, edit the `--symbols` argument in `parse_all.sh`. The default is SPY only. You can add more companys:

   ```bash
   gunzip -d -c $pcap | tcpdump -r - -w - -s 0 | $PYTHON_INTERP src/parse_iex_pcap.py /dev/stdin --symbols SPY,APPL,GOOG,QQQ --trade-date $pcap_date --output-deep-books-too
   ```

4. The parsed data is in `.gz` format. We want to extract it and save it to a `.txt` file which can be feed into Strategy Studio. Run the following command under `iexdownloaderparsers/data/text_tick_data`  ***(please change your symbol and dates accordingly)***:

   ```bash
   gunzip -d -c tick_SPY_20171218.txt.gz | awk -F',' '$4 == "P" {print $0}' > tick_SPY_20171218.txt
   ```

   This command extracts the data and rows where the fourth column is "P", which corresponds to the format of "Depth Update By Price (OrderBook data)" in Strategy Studio. 

   If instead you want to retrive only the trade data, simply change "P" to "T" in the above command, which is following:

   ```bash
   gunzip -d -c tick_SPY_20171218.txt.gz | awk -F',' '$4 == "T" {print $0}' > tick_SPY_20171218.txt
   ```

5. The `tick_SPY_20171218.txt` (*or your custom data file*) is ready to feed in SS.

### Strategy Studio Data Feeds

Strategy Studio offers a great tool for data feed – text tick reader. This allows Strategy Studio to facilitates automated strategy regression testing against pre-canned data scenarios involving common tick types. In this project, we will store these pre-canned data with the specifed naming convention `tick_SYMBOL_YYYYMMDD.txt`. The text reader supports various event types and fields: 

1. **Trades**
2. **Quotes (Best Bid Offer)**
3. Depth Update By Order (OrderBook data)
4. Depth Update By Price (OrderBook data)
5. Order Book Reset
6. Imbalance

For this project, we would mainly focus on two specfic events: **Trades** and **Quotes (BBO)**. 

##### Trades

- Trade data can reveal general trends in world trade, and Strategy Studio provide registration for Trades (and quotes), and we could use the trade data to feed our strategy to test the performance. To sue Trade data, we often implement the function `onTrade` which is called everytime the tick-reader has detected a trade tick. Then, we would use the passed in TradeEventMessage to update our variables, and then decide whether or not we would like to send an order to buy or sell. 

##### Quotes (BBO)

- Quotes, BBO, or Best Bid & Offer provides us the best bid/ask price in the current market. Unlike Trade data, for which the function `onTrade` is specifically called, `onQuote` does not behave the same and is only called when the best new data is updated. In this case, we use the function `onBar` to reach our goal. For strategy studio, on bar allow us to access the tick information within a short amount of period (bar). Everytime, we could access these information to make a trade. 

#### Example CSV Output

<img src="./figs/data_parser_output.jpeg" />



------

## Strategy Implementation

### Kalman Filter Strategy
#### Background
Kalman filter is a common technic for signal processing. It introduces random variances to deduce to potential locations of a projectile. We hope that it could also capture the movements in between trades and profit from it.
#### Implementation
We build a kalman filter that takes the current tick's price and volume. At each timestamp, we first update the filter with observed values, then ask it the predict the next value. We then make trade actions based on the predicted values. The filter is reset at the end of each day because it should capture only continuous values.
#### Results

### Reinforcement Learning Strategy 

## Conclusion & Reflections

### Ruipeng (Ray), Han
1. **What did you specifically do individually for this project?**
2. **What did you learn as a result of doing your project?**
3. **If you had a time machine and could go back to the beginning, what would you have done differently?**
4. **If you were to continue working on this project, what would you continue to do to improve it, how, and why?**
5. **What advice do you offer to future students taking this course and working on their semester long project. Providing detailed thoughtful advice to future students will be weighed heavily in evaluating your responses.**

### Yihong, Jian

1. **What did you specifically do individually for this project?**
2. **What did you learn as a result of doing your project?**
3. **If you had a time machine and could go back to the beginning, what would you have done differently?**
4. **If you were to continue working on this project, what would you continue to do to improve it, how, and why?**
5. **What advice do you offer to future students taking this course and working on their semester long project. Providing detailed thoughtful advice to future students will be weighed heavily in evaluating your responses.**

### Tomoyoshi (Tommy), Kimura

1. **What did you specifically do individually for this project?**
2. **What did you learn as a result of doing your project?**
3. **If you had a time machine and could go back to the beginning, what would you have done differently?**
4. **If you were to continue working on this project, what would you continue to do to improve it, how, and why?**
5. **What advice do you offer to future students taking this course and working on their semester long project. Providing detailed thoughtful advice to future students will be weighed heavily in evaluating your responses.**

### Kaiyuan, Luo

1. **What did you specifically do individually for this project?**
2. **What did you learn as a result of doing your project?**
3. **If you had a time machine and could go back to the beginning, what would you have done differently?**
4. **If you were to continue working on this project, what would you continue to do to improve it, how, and why?**
5. **What advice do you offer to future students taking this course and working on their semester long project. Providing detailed thoughtful advice to future students will be weighed heavily in evaluating your responses.**



## Reference

1. https://www.cmcmarkets.com/en/trading-guides/trading-strategies
2. https://optionalpha.com/help/backtesting-results-summary
3. https://blog.quantinsti.com/backtesting/
4. https://blog.quantinsti.com/stock-market-data-analysis-python/
5. https://plotly.com/
6. https://www.nasdaqtrader.com/content/technicalsupport/specifications/dataproducts/NQTVITCHspecification.pdf

