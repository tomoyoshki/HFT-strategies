#ifdef _WIN32
    #include "stdafx.h"
#endif

#include "TorchStrategy.h"


// create_instance kf_2d_9 KFStrategy UIUC SIM-1001-101 dlariviere 1000000 -symbols SPY
// start_backtest 2022-04-01 2022-04-01 kf_2d_9 1
// export_cra_file /home/vagrant/Desktop/strategy_studio/backtesting/backtesting-results/BACK_kf_test21_2022-10-29_220918_start_04-05-2022_end_04-05-2022.cra  /home/vagrant/Desktop/strategy_studio/backtesting/backtesting-cra-exports

TorchStrategy::TorchStrategy(StrategyID strategyID,
                    const std::string& strategyName,
                    const std::string& groupName):
    Strategy(strategyID, strategyName, groupName) {
        tensor = torch::rand({2, 3});
        std::cout << tensor << std::endl;
}

TorchStrategy::~TorchStrategy() {
}

void TorchStrategy::DefineStrategyParams() {
}

void TorchStrategy::RegisterForStrategyEvents(
    StrategyEventRegister* eventRegister, DateType currDate) {
}

void TorchStrategy::OnTrade(const TradeDataEventMsg& msg) {

}


void TorchStrategy::OnOrderUpdate(const OrderUpdateEventMsg& msg) {
}

void TorchStrategy::OnBar(const BarEventMsg& msg) {
}

void TorchStrategy::AdjustPortfolio() {
}

void TorchStrategy::SendOrder(const Instrument* instrument, int trade_size) {
    
}

void TorchStrategy::OnResetStrategyState() {
}

void TorchStrategy::OnParamChanged(StrategyParam& param) {
}
