#ifdef _WIN32
    #include "stdafx.h"
#endif

#include "TorchStrategy.h"


// create_instance kf_2d_9 KFStrategy UIUC SIM-1001-101 dlariviere 1000000 -symbols SPY
// start_backtest 2022-04-01 2022-04-01 kf_2d_9 1
// export_cra_file /home/vagrant/Desktop/strategy_studio/backtesting/backtesting-results/BACK_kf_test21_2022-10-29_220918_start_04-05-2022_end_04-05-2022.cra  /home/vagrant/Desktop/strategy_studio/backtesting/backtesting-cra-exports


/* Set it to the path to where libtorch is downloaded. */
const std::string path_to_model = "/home/vagrant/ss/sdk/RCM/StrategyStudio/examples/strategies/TorchStrategy/saved_dummy_model.pt";
int current_trade = 0;

TorchStrategy::TorchStrategy(StrategyID strategyID,
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

TorchStrategy::~TorchStrategy() {
}

void TorchStrategy::DefineStrategyParams() {
}

void TorchStrategy::RegisterForStrategyEvents(
    StrategyEventRegister* eventRegister, DateType currDate) {
}

void TorchStrategy::OnTrade(const TradeDataEventMsg& msg) {
    if (current_trade % 3000 == 0) {
        try {
            // Deserialize the ScriptModule from a file using torch::jit::load().
            std::vector<torch::jit::IValue> inputs;
            inputs.push_back(torch::ones({1, 3, 224, 224}));

            // Execute the model and turn its output into a tensor.
            at::Tensor output = model.forward(inputs).toTensor();;
            std::cout << "Decision (0 for sell, 1 for buy):" <<output.index({0}) << ", with trade size: " << output.index({1}) <<"."<< std::endl;
            if (output.index({0}).bool()) {
                int size = output.index({1}).int();
                this->SendSimpleOrder(&msg.instrument(), size);
            } else {
                // Sell. 
                int size = output.index({1}).int();
                this->SendSimpleOrder(&msg.instrument(), -1 * size);
            }
        }
        catch (const c10::Error& e) {
            std::cerr << "error loading the model\n";
        }
    }
    current_trade ++;
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
