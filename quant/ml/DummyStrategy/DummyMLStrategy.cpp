/*================================================================================                               
*     Source: ../RCM/StrategyStudio/examples/strategies/SimpleMomentumStrategy/SimpleMomentumStrategy.cpp                                                        
*     Last Update: 2013/6/1 13:55:14                                                                            
*     Contents:                                     
*     Distribution:          
*                                                                                                                
*                                                                                                                
*     Copyright (c) RCM-X, 2011 - 2013.                                                  
*     All rights reserved.                                                                                       
*                                                                                                                
*     This software is part of Licensed material, which is the property of RCM-X ("Company"), 
*     and constitutes Confidential Information of the Company.                                                  
*     Unauthorized use, modification, duplication or distribution is strictly prohibited by Federal law.         
*     No title to or ownership of this software is hereby transferred.                                          
*                                                                                                                
*     The software is provided "as is", and in no event shall the Company or any of its affiliates or successors be liable for any 
*     damages, including any lost profits or other incidental or consequential damages relating to the use of this software.       
*     The Company makes no representations or warranties, express or implied, with regards to this software.                        
/*================================================================================*/   

/* 
    This is a dummy ML stratgey that uses a simple python model for the purpose of analysis.
    Directions:
        cp DummyMLStrategy.so ~/Desktop/strategy_studio/backtesting/strategies_dlls/
        cd ~/Desktop/strategy_studio/backtesting
        ./StrategyServerBacktesting
        create_instance dummy_NumberOfInstance DummyMLStrategy UIUC SIM-1001-101 dlariviere 10000000 -symbols AAPL
        start_backtest 2022-04-06 2022-04-07 DummyMLStrategy 1

    Export Backtest Results:
        export_cra_file backtesting-results/BACK_mean_reversion42_2022-05-13_095131_start_04-06-2022_end_04-07-2022.cra backtesting-cra-exports
*/

#ifdef _WIN32
    #include "stdafx.h"
#endif

#include "DummyMLStrategy.h"
#include <torch/torch.h>
#include <torch/script.h>

#include "FillInfo.h"
#include "AllEventMsg.h"
#include "ExecutionTypes.h"
#include <Utilities/Cast.h>
#include <Utilities/utils.h>

#include <math.h>
#include <iostream>
#include <cassert>
#include <utility>
#include <string>
using namespace RCM::StrategyStudio;
using namespace RCM::StrategyStudio::MarketModels;
using namespace RCM::StrategyStudio::Utilities;

using namespace std;
/* Set it to the path to where libtorch is downloaded. */
const std::string path_to_model = "/home/vagrant/Desktop/strategy_studio/localdev/RCM/StrategyStudio/examples/strategies/DummyMLStrategy/saved_dummy_model.pt";

DummyMLStrategy::DummyMLStrategy(StrategyID strategyID,
const std::string& strategyName, const std::string& groupName):
    Strategy(strategyID, strategyName, groupName){

    try {
        // Deserialize the ScriptModule from a file using torch::jit::load().
        model = torch::jit::load(path_to_model);
    }
    catch (const c10::Error& e) {
        std::cerr << "error loading the model\n";
    }

}

DummyMLStrategy::~DummyMLStrategy() {
}

void DummyMLStrategy::OnResetStrategyState() {
}

void DummyMLStrategy::DefineStrategyParams() {
}

void DummyMLStrategy::DefineStrategyCommands() {
}

void DummyMLStrategy::RegisterForStrategyEvents(
    StrategyEventRegister* eventRegister, DateType currDate) {
    for (SymbolSetConstIter it = symbols_begin(); it != symbols_end(); ++it) {
        eventRegister->RegisterForBars(*it, BAR_TYPE_TIME, 1);
    }
}

void DummyMLStrategy::OnTrade(const TradeDataEventMsg& msg) {
    try {
        // Deserialize the ScriptModule from a file using torch::jit::load().
        std::vector<torch::jit::IValue> inputs;
        inputs.push_back(torch::ones({1, 3, 224, 224}));

        // Execute the model and turn its output into a tensor.
        at::Tensor output = model.forward(inputs).toTensor();;

        std::cout << "Decision (0 for sell, 1 for buy):" <<output.index({0}) << ", with trade size: " << output.index({1}) <<"."<< std::endl;
        
        /* TODO: Handles buy or sell */
    }
    catch (const c10::Error& e) {
        std::cerr << "error loading the model\n";
    }

}

void DummyMLStrategy::OnBar(const BarEventMsg& msg) {
    return;
}

void DummyMLStrategy::OnOrderUpdate(const OrderUpdateEventMsg& msg) {
}

void DummyMLStrategy::AdjustPortfolio(const Instrument* instrument,
int desired_position) {
}

void DummyMLStrategy::SendSimpleOrder(const Instrument* instrument,
int trade_size) {
}


void DummyMLStrategy::SendOrder(const Instrument* instrument,
int trade_size) {
    return;
}

void DummyMLStrategy::RepriceAll() {
}

void DummyMLStrategy::Reprice(Order* order) {

}

void DummyMLStrategy::OnStrategyCommand(
    const StrategyCommandEventMsg& msg) {
    switch (msg.command_id()) {
        case 1:
            RepriceAll();
            break;
        case 2:
            trade_actions()->SendCancelAll();
            break;
        default:
            logger().LogToClient(LOGLEVEL_DEBUG, "Unknown command received");
            break;
    }
}

void DummyMLStrategy::OnParamChanged(StrategyParam& param) {
}
