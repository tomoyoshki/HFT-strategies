#ifdef _WIN32
    #include "stdafx.h"
#endif

#include "LSTMStrategy.h"


// create_instance kf_2d_9 KFStrategy UIUC SIM-1001-101 dlariviere 1000000 -symbols SPY
// start_backtest 2022-04-01 2022-04-01 kf_2d_9 1
// export_cra_file /home/vagrant/Desktop/strategy_studio/backtesting/backtesting-results/BACK_kf_test21_2022-10-29_220918_start_04-05-2022_end_04-05-2022.cra  /home/vagrant/Desktop/strategy_studio/backtesting/backtesting-cra-exports


/* Set it to the path to where libtorch is downloaded. */
const std::string path_to_model = "/home/vagrant/ss/sdk/RCM/StrategyStudio/examples/strategies/LSTMStrategy/lstm.pt";
int current_trade = 0;

LSTMStrategy::LSTMStrategy(StrategyID strategyID,
                    const std::string& strategyName,
                    const std::string& groupName):
    Strategy(strategyID, strategyName, groupName) {
    try {
        // Deserialize the ScriptModule from a file using torch::jit::load().
        model = torch::jit::load(path_to_model);
    }
    catch (const c10::Error& e) {
        std::cerr << "error loading the model\n";
    }
}

LSTMStrategy::~LSTMStrategy() {
}

void LSTMStrategy::DefineStrategyParams() {
}

void LSTMStrategy::RegisterForStrategyEvents(
    StrategyEventRegister* eventRegister, DateType currDate) {
}

void LSTMStrategy::OnTrade(const TradeDataEventMsg& msg) {
     // For lack of a better model, just execute an order every 6000 trades.
     if (current_trade % 6000 == 0) {
        try {
            // Deserialize the ScriptModule from a file using torch::jit::load().
            std::vector<torch::jit::IValue> inputs;
            inputs.push_back(torch::ones({1, 3, 224, 224}));

            // Execute the model and turn its output into a tensor.
            at::Tensor output = model.forward(inputs).toTensor();;
            std::cout << "Decision (0 for sell, 1 for buy):" <<output.index({0}) << ", with trade size: " << output.index({1}) <<"."<< std::endl;
            if (output.index({0}).item<bool>()) {
                int size = output.index({1}).item<int>();
                this->SendSimpleOrder(&msg.instrument(), size);
            } else {
                // Sell. 
                int size = output.index({1}).item<int>();
                this->SendSimpleOrder(&msg.instrument(), -1 * size);
            }
        }
        catch (const c10::Error& e) {
            std::cerr << "error loading the model\n";
        }
    }
    current_trade ++;
}


void LSTMStrategy::OnOrderUpdate(const OrderUpdateEventMsg& msg) {
}

void LSTMStrategy::OnBar(const BarEventMsg& msg) {
}

void LSTMStrategy::AdjustPortfolio() {
}

void LSTMStrategy::SendSimpleOrder(const Instrument* instrument,
int trade_size) {
    // send order two pennies more aggressive than BBO
    double m_aggressiveness = 0.02;
    double last_trade_price = instrument->last_trade().price();
    double price = trade_size > 0 ? last_trade_price + m_aggressiveness :
    last_trade_price - m_aggressiveness;
    OrderParams params(*instrument,
        abs(trade_size),
        price,
        (instrument->type() == INSTRUMENT_TYPE_EQUITY) ? MARKET_CENTER_ID_IEX :
        ((instrument->type() == INSTRUMENT_TYPE_OPTION) ?
        MARKET_CENTER_ID_CBOE_OPTIONS : MARKET_CENTER_ID_CME_GLOBEX),
        (trade_size > 0) ? ORDER_SIDE_BUY : ORDER_SIDE_SELL,
        ORDER_TIF_DAY,
        ORDER_TYPE_LIMIT);

    std::cout << "SendSimpleOrder(): about to send new order for " <<
     trade_size << " at $" << price << std::endl;
    TradeActionResult tra = trade_actions()->SendNewOrder(params);
    if (tra == TRADE_ACTION_RESULT_SUCCESSFUL) {
        // m_instrument_order_id_map[instrument] = params.order_id;
        std::cout << "SendOrder(): Sending new order successful!" << std::endl;
    } else {
        std::cout << "SendOrder(): Error sending new order!!!" << tra
        << std::endl;
    }
}


void LSTMStrategy::SendOrder(const Instrument* instrument, int trade_size) {
    
}

void LSTMStrategy::OnResetStrategyState() {
}

void LSTMStrategy::OnParamChanged(StrategyParam& param) {
}
