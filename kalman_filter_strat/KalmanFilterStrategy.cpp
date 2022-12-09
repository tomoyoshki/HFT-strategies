#ifdef _WIN32
    #include "stdafx.h"
#endif

#include "KalmanFilterStrategy.h"
#include <math.h>
#include <iostream>
#include <cassert>
#include <ctime>

// cp KalmanFilterStrategy.so /home/vagrant/Desktop/strategy_studio/backtesting/strategies_dlls/
// create_instance kf_on_4 KFStrategy UIUC SIM-1001-101 dlariviere 1000000 -symbols SPY
// start_backtest 2022-04-05 2022-04-06 kf_on_4 0
// export_cra_file /home/vagrant/Desktop/strategy_studio/backtesting/backtesting-results/BACK_kf_test21_2022-10-29_220918_start_04-05-2022_end_04-05-2022.cra  /home/vagrant/Desktop/strategy_studio/backtesting/backtesting-cra-exports

KFStrategy::KFStrategy(StrategyID strategyID,
                    const std::string& strategyName,
                    const std::string& groupName):
    Strategy(strategyID, strategyName, groupName) {

    int n = 2; // Number of states
    int m = 2; // Number of measurements

    double dt = 1e-6; // Time step

    Eigen::MatrixXd A(n, n); // System dynamics matrix
    Eigen::MatrixXd C(m, n); // Output matrix
    Eigen::MatrixXd Q(n, n); // Process noise covariance
    Eigen::MatrixXd R(m, m); // Measurement noise covariance
    Eigen::MatrixXd P(n, n); // Estimate error covariance

    // Discrete LTI projectile motion, measuring position only
    A << 1, 0, 0, 1;
    C << 1, 0, 0, 1;

    // Reasonable covariance matrices
    Q << .09, .07, .05, .04;
    R << 2, 0, 0, 1;
    P << 10, 5, 2, 1;
    

    kf = unique_ptr<KalmanFilter>(new KalmanFilter(1, A, C, Q, R, P));
    // kf = new KalmanFilter();
    kalman_initialized = false;

    y = unique_ptr<Eigen::VectorXd>(new Eigen::VectorXd(m));

    amount = 0;
}

KFStrategy::~KFStrategy() {
}

void KFStrategy::DefineStrategyParams() {
    kf->init();
}

void KFStrategy::RegisterForStrategyEvents(
    StrategyEventRegister* eventRegister, DateType currDate) {
        std::cout << "Registrating for the strategy event" << std::endl;
}

void KFStrategy::OnTrade(const TradeDataEventMsg& msg) {

    // get the current time
    // date msg_date = msg.adapter_time().date();
    tm date_tm = to_tm(msg.adapter_time());
    // if time is near the end of the trade, reset kalman filter, and sell everything
    if (date_tm.tm_hour == 19 && date_tm.tm_min >= 59 && kalman_initialized) {
        kalman_initialized = false;
        this->SendOrder(&msg.instrument(), -1 * amount);
        amount = 0;
        trade_number = 0;
        return;
    }

    // if kalman is initialzied
    if (kalman_initialized) {
        // Update the state and get the next state
        *y << msg.trade().price(), msg.trade().size();
        kf->update(*y);
        // Only start kalman filter after 100 rounds for intialization
        if (trade_number > 100) {
            Eigen::VectorXd current_state = kf->state();
            float curr_eq = msg.trade().price() * msg.trade().size();

            // get the relative difference
            float rel_diff = (current_state[0] - msg.trade().price()) / msg.trade().price();
            long order_size = msg.trade().size() > current_state[1] ? current_state[1] : msg.trade().size();

            // set likelihood to conduct a trade
            double r = ((double) rand() / (RAND_MAX));

            // if the difference is big enough such that we could make profit on
            if ( rel_diff > 1e-3 && r > 0.5) {
                this->SendOrder(&msg.instrument(), order_size);
                amount += order_size;
            } else if (rel_diff < -1e-3 && amount > 0 && r > 0.5) {
                this->SendOrder(&msg.instrument(), -1*min(order_size, amount));
                amount -= min(order_size, amount);
            }
        }
    } else {
        // initialize kalman filter
        x0 = unique_ptr<Eigen::VectorXd>(new Eigen::VectorXd(2));
        *x0 << msg.trade().price(), msg.trade().size();
        kf->init(0, *x0);
        kalman_initialized = true;
    }
    trade_number++;
}


void KFStrategy::OnOrderUpdate(const OrderUpdateEventMsg& msg) {
}

void KFStrategy::OnBar(const BarEventMsg& msg) {
}

void KFStrategy::AdjustPortfolio() {
}

void KFStrategy::SendOrder(const Instrument* instrument, int trade_size) {
    double last_trade_price = instrument->last_trade().price();
    double price = trade_size > 0 ? last_trade_price : last_trade_price;
    OrderParams params(*instrument,
        abs(trade_size),
        price,
        (instrument->type() == INSTRUMENT_TYPE_EQUITY) ? MARKET_CENTER_ID_IEX :
            ((instrument->type() == INSTRUMENT_TYPE_OPTION) ?
            MARKET_CENTER_ID_CBOE_OPTIONS : MARKET_CENTER_ID_CME_GLOBEX),
        (trade_size > 0) ? ORDER_SIDE_BUY : ORDER_SIDE_SELL,
        ORDER_TIF_DAY,
        ORDER_TYPE_LIMIT);
    TradeActionResult tra = trade_actions()->SendNewOrder(params);
}

void KFStrategy::OnResetStrategyState() {
}

void KFStrategy::OnParamChanged(StrategyParam& param) {
}
