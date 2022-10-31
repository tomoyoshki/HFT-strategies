#ifdef _WIN32
    #include "stdafx.h"
#endif

#include "KalmanFilterStrategy.h"

KFStrategy::KFStrategy(StrategyID strategyID,
                    const std::string& strategyName,
                    const std::string& groupName):
    Strategy(strategyID, strategyName, groupName) {
    KalmanFilter kf();
}

KFStrategy::~KFStrategy() {
}

void KFStrategy::DefineStrategyParams() {
}

void KFStrategy::RegisterForStrategyEvents(
    StrategyEventRegister* eventRegister, DateType currDate) {

}

void KFStrategy::OnTrade(const TradeDataEventMsg& msg) {
}


void KFStrategy::OnOrderUpdate(const OrderUpdateEventMsg& msg) {
}

void KFStrategy::OnBar(const BarEventMsg& msg) {
}

void KFStrategy::AdjustPortfolio() {
}

void KFStrategy::SendOrder(const Instrument* instrument, int trade_size) {
}

void KFStrategy::OnResetStrategyState() {
}

void KFStrategy::OnParamChanged(StrategyParam& param) {
}
