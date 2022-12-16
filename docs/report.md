# High Frequency Trading Strategies with RCM Strategy Studio

[[_TOC_]]

**Group 1 FIN 556 High Frequency Trading Final Report**

## People

**Ruipeng (Ray), Han**
- TODO

**Yihong, Jian**

- Yihong is a senior studying computer science at University of Illinois, Urbana-Champaign graduating in May 2023. He is passionate about lower level programmings with experince in multiple fields such as networks, distributed systems, and kernels. He has multiple project experiences with C++, such as writing a HDFS from scratch. 

**Tomoyoshi (Tommy), Kimura**

- Tommy is a current Junior in Computer Science at the University of Illinois at Urbana-Champaign. Tommy plans to graduate in his Junior year to pursue a degree in Master of Science in Computer Science. Tommy is passionate about the intersection of Systems and Intelligence, specifically about the system protocols for efficient, scalable, and robust distributed learning. Tommy enjoys learning, and he is interested in Systems engineering, High Frequency Trading, Machine Learning, Front End Development, Backend Development, and Human Computer Interaction.

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

#### Background

### Long Short Term Memory based Strategy

#### Background 

Long Short-Term Memory (LSTM) is a type of recurrent neural network (RNN) that is well-suited to model long-term dependencies in data. RNNs are neural networks that process sequential data, such as text or time series data, by making use of internal memory to remember information from previous time steps. LSTMs are a variant of RNNs that are able to capture long-term dependencies more effectively than traditional RNNs, by using gating mechanisms to control the flow of information in the network.

#### Implementation

Since Strategy Studio does not have native torch library, we had to figure out a way to install torch library in C++. To implemennt LSTM based strategy, we first trained our LSTM model in Python against SPY data from January to mid May, then using Pytorch built in tool to trace the model. In C++, we then load this model and use this model to inference on the current price to predict the price for the next trade.

##### Python Implementation Details

We trained our model in Python.

**Dataset**
- We are using SPY Jan 03 - May 13 stock market data

**Loss**

**Test**


#### Results

## Analysis

### Interrpetation & Visualization Layout

```
Analysis
	├── compare_strategy.py
	├── main.py
	├── strategy_analysis.py
```

### Interpretation

- There are various metrics that we would like to evaluate on our result. Strategy Studio outputs three files: fills, orders, and PnL (Profit and Loss). For analysis, we would mainly focus on PnL since the net loss is what we care about the most as traders. 

- The PnL file generated is a `.csv` file with three columns: Strategy name, Time, and Cumulative PnL. For interpretation, we would analyze the PnL based on several metrics. 

  - Maximum Profit and Loss

    <img src="./figs/max_pnl.png" />

  - Minimum Profit and Loss

    <img src="./figs/min_pnl.png" />

  - Net Profit and Loss

    <img src="./figs/net_pnl.png" />

  - Cumulative Returns

    <img src="./figs/cumulative_pnl.png" />

  - Sharpe Ratio

    <img src="./figs/sharpe.png" />

  - Max Drowndown

    <img src="./figs/max_drown.png" />

### Visualization

- For visualization, we are using the Python package `plotly` to generate various meaningful graphs for evaluation. `plotly` offers powerful interactive figures for users to modify directly. Just like a few of the examples in the previous sections, we could see that there are various types like line, bar, and table like figures that simulates stock market graphs in real finance applications like Yahoo Finance. 
- Plotly is extremely power for interactive visualization. For example, we could see that we could decide which trace of the graph we would like to see, and hide the other one by simply clicking the legend. 

#### Implementation

There are mainly two classes: `StrategyAnalysis` and `CompareStrategy` . We also have a `main.py` file for automation, that is, our DevOps could directly run this program that generates the newest export files, and our program would look for this latest files and execute on these. Then, it would store these files in a new directory for us to access. Beside automation, our implementation also allows interactive modes that give the user control over the exact strategy they would like to compare. User could add as many as they want by Name, Id, and Ticks. At the end of the interactive mode, the program would automatically run the comparison class and generates outputs. 

##### Strategy Analysis

- This class represents a Strategy object. This type of object is initialized with the three files generated from the Strategy Studio and it would use Pandas Dataframe to store each fill, order, and PnL. For each strategy, we also add the corresponding Tick data into it so we could evaluate our strategy with the market data to understand the reasoning behind the rise and the fall of each. 
- Examples could be found in previous sections. 

##### Compare Strategy

- The strategy acts like a container that holds all the Strategy object we mentioned earlier. In this class, we could evaluate the performance of each strategy with each other, and output graph and table for traders to analyze each strategy. The measurement table for Swing between Apple and SPY could be an example, and another example is the evaluation of BLSF Strategy and Swing Strategy on SPY market data. 
  <img src="./figs/Swing_BLSF_SPY.png" />

#### Interpretation & Visualization Usage

- Direct to `analysis` directory (`cd ./analysis`)
- Simply run `python3 main.py` would run visualization by using the latest three files (Fill, Order, and PnL). This will generate figures and store in `./figs/` directory. 
- There is also an interactive version by runing `python3 main.py -i` and follows the promot.
  - Interactive mode would ask you to add strategy by entering the 
    - `Name` of the strategy
    - `ID` of the strategy output if the strategy is ran multiple times
    - `Ticks` of the strategy, or the symbol, for example:`SPY` 
  - When we enter no for adding strategy, the interactive mode would also outputs the **measurement table** with each strategy statistics as a column

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

copied from ie498 TOOD change stuff

1. **What did you specifically do individually for this project?**

   For this project, the major tasks I was responsible for were Analysis & Visualization, Strategies Implementation, Documentations, and video recordings. The entire project has been splitted into different phases, and during the first phase, each of us worked on different parts, and I mainly developed the analysis program for the strategies. I was able to design and implement various objects with Python to achieve the goal, along with learning and applying powerful packages like Plotly to create good visualizations for our strategy analysis. In the last phases, I have used this program to generate various meaningful graphs for my peer and for the documentation. 

   During the second phase, I took over the responsibility to dive into the Strategy Studio examples to understand the interface of the class Strategy, and was the first one to try out implementing an introductory strategy recommended by the Professor (Buy Last Sell First). During this process, I had the opportunities to help my team to understand how the Strategy class interface works, and give them some guidance in helping them to start off on their strategy. I was also able to test the parser we developed in the earlier phase and pointed out potential possibilities for improvements. Beside BLSF, I also tried writing a different strategy, the Swing Strategy. I found this strategy through internet and learned from videos to implement it in Strategy Studio. 

   For documentation, I also worked on organizing readme file and the Final report for this project, including outlining the project structures for my peers to fill in and polishing the files according to Professor’s rubric.  

   Lastly, I have recorded the Demo video for project submission. 

2. **What did you learn as a result of doing your project?**

   I have learned a lot as a result of doing this project. And one thing that I am most grateful about is learning the DevOps pipeline for a better project flow. I was able to learn a lot from my teammates regarding better Git control through a strict pipeline checking for every commit. I feel like I am more comfortable using Git for any project and believe that this has benefited me for the future. Beside the general DevOps, I have also learned and firmed what I have learned in the class: Market Trading. From developing my own Trading Strategies over financial market, I feel like I now have a solid understanding in the microstructure of the market, including the data feed like Trade and Best Bid and Offer. Writing my own strategies also taught me many tricks that this industry has for making profits. This project definitely prepares me well for this area in many ways, and also excites me for the course **Algorithmic Market Microstructure** next semester. I have also practiced my skills in Object Programming and Data Visualization  through this project, and these would definitely be beneficial to my future. 

3. **If you had a time machine and could go back to the beginning, what would you have done differently?**

   One thing that I would have done differently is to make sure that I have done enough research and negotiate with team member enough to have a better understanding of the detail pipelines of the project. And I would definitely recommend conducting more tests for each sub-project. When I was generating graphs with my analysis program that I wrote in the earlier stages, I realized that I missed some small features and resulted in errors or generated some figures that were under the expectation. Because of this, I had to renegotiate with my team members and rewrite some parts of my program to fit into the automation pipeline as well as fixing the errors for generating the stategy figures. Other thing that I would have done differently is the work distribution during this semester. It was a bit rush near the end and things have only gotten busier with finals and other final projects. Therefore, distributing more heavy works to the first half of the semester would definately improve it. 

4. **If you were to continue working on this project, what would you continue to do to improve it, how, and why?**

   If I were to continue working on this project, I would continue working on writing better strategies and more meaningful visualizations. Although we have already implemented different strategies, there are definitely various cases that these strategies are prone to, and there are definitely different features we could implement to improve the performance of the strategy. The current strategy I have implemented takes bold moves and perform well with data that has oscillating behaviors. However, when the market data falls continuously, my strategy does not perform well and loses money, since our assumption is broken in this case. Due to the lack of time, I have yet been able to modify much to generate a satisfying output, but I would love to continue working on it for better performance. Beside this strategy, I feel like I could improve the analysis & visualization program further to reflect on possibilities for improvements of the strategies. The current program would only generate some statistical measurements and general PnL of the strategies with the tick data. However, if the program could show graphs that give feedbacks on how to improve the strategies, the process of writing better strategies will be more efficient and less time-consuming. 

5. **What advice do you offer to future students taking this course and working on their semester long project. Providing detailed thoughtful advice to future students will be weighed heavily in evaluating your responses.**

   The first advice I would give is to negotiate with the teammate well and develop a strong DevOps and project pipeline in the beginning of the semester long project. Having this strong pipeline allows a smoother workflow and improves the project efficiency in many ways. Generating these rules could assure the code quality as well as the project standard in many ways. My second advice would be make sure that the group has a very detailed and structured plan for their project. This would make sure that every team member can keep track of whether what they are doing fit into the project pipeline, and this would reduce lots of error and time-consuming debugging process near the end of the project. Lastly, I would recommend that never hesitate to ask questions to both your team members and the professor. Your peers could often give you new insights to the problems, and you could always learn something from them. Professor Lariviere is very informed in this industry, and he always reply fast and know answers to most of your questions. 

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

