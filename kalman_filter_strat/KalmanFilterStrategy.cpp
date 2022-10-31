#ifdef _WIN32
    #include "stdafx.h"
#endif

#include "KalmanFilterStrategy.h"

KFStrategy::KFStrategy(StrategyID strategyID,
                    const std::string& strategyName,
                    const std::string& groupName):
    Strategy(strategyID, strategyName, groupName) {

    int n = 3; // Number of states
    int m = 1; // Number of measurements

    double dt = 1.0/30; // Time step

    Eigen::MatrixXd A(n, n); // System dynamics matrix
    Eigen::MatrixXd C(m, n); // Output matrix
    Eigen::MatrixXd Q(n, n); // Process noise covariance
    Eigen::MatrixXd R(m, m); // Measurement noise covariance
    Eigen::MatrixXd P(n, n); // Estimate error covariance

    // Discrete LTI projectile motion, measuring position only
    A << 1, dt, 0, 0, 1, dt, 0, 0, 1;
    C << 1, 0, 0;

    // Reasonable covariance matrices
    Q << .05, .05, .0, .05, .05, .0, .0, .0, .0;
    R << 5;
    P << .1, .1, .1, .1, 10000, 10, .1, 10, 100;

    kf = new KalmanFilter(dt, A, C, Q, R, P);
    kalman_initialized = false;

    y = new Eigen::VectorXd(m);
}

KFStrategy::~KFStrategy() {
    free(kf);
}

void KFStrategy::DefineStrategyParams() {
    kf->init();
}

void KFStrategy::RegisterForStrategyEvents(
    StrategyEventRegister* eventRegister, DateType currDate) {
        std::cout << "Registrating for the strategy event" << std::endl;
}

void KFStrategy::OnTrade(const TradeDataEventMsg& msg) {
    if (kalman_initialized) {
        *y << msg.trade().price();
        kf->update(*y);
        if (trade_number > 100) {
            Eigen::VectorXd current_state = kf->state();
            // if the next state is greater than the current price, price is going to rise, buy!
            float rel_diff = (current_state[0] - msg.trade().price()) / msg.trade().price();
            std::cout << "Relative difference: " << rel_diff << std::endl;
            if ( rel_diff > 1e-5) {
                this->SendOrder(&msg.instrument(), 1);
            } else if (rel_diff < -1e-5) {
                this->SendOrder(&msg.instrument(), -1);
            }
        }
    } else {
        x0 = new Eigen::VectorXd(3);
        *x0 << msg.trade().price(), 0.1, -0.1;
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
    std::cout << "SendTradeOrder(): about to send new order for "
                << trade_size
                << " at $"
                << price
                << std::endl;
    TradeActionResult tra = trade_actions()->SendNewOrder(params);
    if (tra == TRADE_ACTION_RESULT_SUCCESSFUL) {
        std::cout << "Sending new trade order successful!" << std::endl;
    } else {
        std::cout << "Error sending new trade order..." << tra << std::endl;
    }
}

void KFStrategy::OnResetStrategyState() {
}

void KFStrategy::OnParamChanged(StrategyParam& param) {
}
