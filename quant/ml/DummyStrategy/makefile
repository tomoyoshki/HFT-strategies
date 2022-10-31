# Conditional settings based on passed in variables
ifdef INTEL
    CC=icc
else
    CC=g++
endif

ifdef DEBUG
    CFLAGS=-c -g -fPIC -fpermissive -std=c++11 -std=gnu++14
else
    CFLAGS=-c -fPIC -fpermissive -O3 -std=c++11 -std=gnu++14
endif

LIBPATH=../../../libs/x64
INCLUDEPATH=../../../includes

######
# LIBTORCHPATH is added by Ruipeng. This can compile but it won't pass
# the Strategy Studio check when creating instance in backtest command line.
#####
LIBTORCHPATH =/home/vagrant/Desktop/strategy_studio/localdev/RCM/StrategyStudio/examples/strategies/DummyMLStrategy/libtorch
INCLUDES=-I/usr/include -I$(INCLUDEPATH) -I$(LIBTORCHPATH)/include -I${LIBTORCHPATH}/include/torch/csrc/api/include 


LDFLAGS=$(LIBPATH)/libstrategystudio_analytics.a $(LIBPATH)/libstrategystudio.a $(LIBPATH)/libstrategystudio_transport.a $(LIBPATH)/libstrategystudio_marketmodels.a $(LIBPATH)/libstrategystudio_utilities.a $(LIBPATH)/libstrategystudio_flashprotocol.a
LIBRARY=DummyMLStrategy.so

SOURCES=DummyMLStrategy.cpp
HEADERS=DummyMLStrategy.h

OBJECTS=$(SOURCES:.cpp=.o)

all: $(HEADERS) $(LIBRARY)

$(LIBRARY) : $(OBJECTS)
	$(CC) -shared -Wl,-soname,$(LIBRARY).1 -o $(LIBRARY) $(OBJECTS) $(LDFLAGS)
	
.cpp.o: $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf *.o $(LIBRARY)

run_backtest: all
	cd ~/Downloads/ss_backtesting ; echo $PWD ; ./run_backtest.sh

output_results: 
	export CRA_RESULT=`cd ~/Downloads/ss_backtesting ; find ./backtesting-results -name 'BACK*cra' | tail -n 1` ; \
	echo $$CRA_RESULT
	export TEST_VAR="hi" ; \
	echo $$TEST_VAR
	
