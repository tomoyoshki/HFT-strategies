# FIN 556 Group 1 Project

## Team Members

- Ruipeng Han - ruipeng2@illinois.edu
- Yihong Jian - yihongj2@illinois.edu
- Tomoyoshi Kimura - tkimura4@illinois.edu (Team Leader)
- Kaiyuan Luo - kaiyuan8@illinois.edu


## Project Description

We will implement HFT strategies that trade on real-world market data using proprietary software Strategy Studio (SS) for the final project. This goal has several components: market data, strategy, analysis, and automation. The first component is data. Since SS does not have built-in data, we need to harvest our own for backtesting. Therefore, we will be downloading historic market data from IEX. The second component is the strategy itself--implement trading strategies and run them in SS. We are going to implement two strategies for this project, Kalman Filter and Reinforcement Learning. The next component will be interpreting the output of SS. SS will generate complex CSV files containing trade histories and earnings. Since the CSVs do not provide much explicitly, we will interpret them and analyze the strategies' performance, generating comparisons and visualizations. The final component is automation. Since running SS backtesting requires users to type many lines of commands, we feel the need to write a script to replace the manual work. Ideally, we should be able to run the backtesting in one click.

## Components & Technologies:

### Data parsing

- We will be using the IEX parser provided by Professor David Lariviere.

### Implement strategy

- Strategies will be implemented by extending StrategyStudio::Strategy interface using C++. In this way, we can compile binaries for SS to backtest.
  - C++
  - Strategy Studio

### Analysis

- Since analysis requires parsing output CSV files and generating plots, we determined that Python will best serve our interest due to the widely available data analysis packages. Some potential tools are:
  - Matplotlib
  - Seaborn
  - Pandas
  - Numpy

### Automation

- SS works by launching a separate terminal where users need to type in commands. Therefore, a logical approach to writing an automation script will be redirecting stdin and stdout. We feel that the two viable options are bash scripts using directional operators or Python with system calls.
  - Bash script
  - python script

## Evaluation & Goal:

- Clarity & understandability
  - Document process and week meetings
  - Codes are readable and follow google code styles standard
  - Proper use of pull requests
- Data
  - Can generate usable data
  - Runtime is reasonable
- Strategy
  - Strategy can run
  - Strategy outperforms holding the ticker
  - Strategy outperforms ExampleStrategy
- Interpretation & Visualization
  - Produce human readable report from raw csv
  - Graphs are clear and understandable