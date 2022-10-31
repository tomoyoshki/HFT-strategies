/**
 * @file DummyMLStrategy.h
 * @author Ruipeng Han
 * @brief 
 * @version 0.1
 * @date 2022-10-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#ifndef _STRATEGY_STUDIO_LIB_EXAMPLES_SIMPLE_MOMENTUM_STRATEGY_H_
#define _STRATEGY_STUDIO_LIB_EXAMPLES_SIMPLE_MOMENTUM_STRATEGY_H_

#ifdef _WIN32
    #define _STRATEGY_EXPORTS __declspec(dllexport)
#else
    #ifndef _STRATEGY_EXPORTS
    #define _STRATEGY_EXPORTS
    #endif
#endif

#include <Strategy.h>
#include <Analytics/ScalarRollingWindow.h>
#include <Analytics/InhomogeneousOperators.h>
#include <Analytics/IncrementalEstimation.h>
#include <MarketModels/Instrument.h>
#include <Utilities/ParseConfig.h>
#include <torch/torch.h>
#include <torch/script.h>

#include <vector>
#include <map>
#include <iostream>
#include <string>

using namespace RCM::StrategyStudio;

class DummyMLStrategy : public Strategy {
 public:
    DummyMLStrategy(StrategyID strategyID,
    const std::string& strategyName, const std::string& groupName);
    ~DummyMLStrategy();

 public:  /* from IEventCallback */
    /**
     * This event triggers whenever trade message arrives from a market data source.
     */ 
    virtual void OnTrade(const TradeDataEventMsg& msg);

    /**
     * This event triggers whenever aggregate volume at best price changes, based 
     * on the best available source of liquidity information for the instrument.
     *
     * If the quote datasource only provides ticks that change the NBBO, top quote will be set to NBBO
     */ 
    virtual void OnTopQuote(const QuoteEventMsg& msg) {}

    /**
     * This event triggers whenever a new quote for a market center arrives from a consolidate or direct quote feed,
     * or when the market center's best price from a depth of book feed changes.
     *
     * User can check if quote is from consolidated or direct, or derived from a depth feed. This will not fire if
     * the data source only provides quotes that affect the official NBBO, as this is not enough information to accurately
     * mantain the state of each market center's quote.
     */ 
    virtual void OnQuote(const QuoteEventMsg& msg) {}

    /**
     * This event triggers whenever a order book message arrives. 
     * This will be the first thing that triggers if an order book entry 
     * impacts the exchange's DirectQuote or Strategy Studio's TopQuote calculation.
     */ 
    virtual void OnDepth(const MarketDepthEventMsg& msg) {}

    /**
     * This event triggers whenever a Bar interval completes for an instrument
     */ 
    virtual void OnBar(const BarEventMsg& msg);

    /**
     * This event contains alerts about the state of the market
     */
    virtual void OnMarketState(const MarketStateEventMsg& msg) {}

    /**
     * This event triggers whenever new information arrives about a 
     * strategy's orders
     */ 
    virtual void OnOrderUpdate(const OrderUpdateEventMsg& msg);

    /**
     * This event contains strategy control commands arriving from the Strategy 
     * Studio client application (eg Strategy Manager)
     */ 
    virtual void OnStrategyControl(const StrategyStateControlEventMsg& msg) {}

    /**
     *  Perform additional reset for strategy state 
     */
    void OnResetStrategyState();

    /**
     * This event contains alerts about the status of a market data source
     */ 
    void OnDataSubscription(const DataSubscriptionEventMsg& msg) {}

    /**
     * This event triggers whenever a custom strategy command is sent 
     * from the client
     */ 
    void OnStrategyCommand(const StrategyCommandEventMsg& msg);

    /**
     * Notifies strategy for every succesfull change 
     * in the value of a strategy parameter.
     */ 
    void OnParamChanged(StrategyParam& param);

 private:  // Helper functions specific to this strategy
    void AdjustPortfolio(const Instrument* instrument, int desired_position);
    void SendOrder(const Instrument* instrument, int trade_size);
    void SendSimpleOrder(const Instrument* instrument, int trade_size);
    void RepriceAll();
    void Reprice(Order* order);

 private:  /* from Strategy */
    virtual void RegisterForStrategyEvents(StrategyEventRegister* eventRegister,
    DateType currDate);

    /**
     * Define any params for use by the strategy 
     */     
    virtual void DefineStrategyParams();

    /**
     * Define any strategy commands for use by the strategy
     */ 
    virtual void DefineStrategyCommands();

 private:
    /* Pytorch model */
    torch::jit::script::Module model;
};

extern "C" {

    _STRATEGY_EXPORTS const char* GetType() {
        return "DummyMLStrategy";
    }

    _STRATEGY_EXPORTS IStrategy* CreateStrategy(const char* strategyType,
                                   unsigned strategyID,
                                   const char* strategyName,
                                   const char* groupName) {
        if (strcmp(strategyType, GetType()) == 0) {
            return *(new DummyMLStrategy(strategyID, strategyName,
            groupName));
        } else {
            return NULL;
        }
    }

    // must match an existing user within the system
    _STRATEGY_EXPORTS const char* GetAuthor() {
        return "dlariviere";
    }

    // must match an existing trading group within the system
    _STRATEGY_EXPORTS const char* GetAuthorGroup() {
        return "UIUC";
    }

    _STRATEGY_EXPORTS const char* GetReleaseVersion() {
        return Strategy::release_version();
    }
}

#endif
