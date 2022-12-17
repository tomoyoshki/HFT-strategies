# Conditional settings based on passed in variables
ifdef INTEL
    CC=icc
else
    CC=g++
endif

# see get_torch.sh on how obtain pytorch
# Must use c++11 ABI version
TORCH_PATH=/home/vagrant/ss/sdk/RCM/StrategyStudio/includes/libtorch
ifdef DEBUG
    CFLAGS=-c -g -fPIC -fpermissive -std=c++11
else
    CFLAGS=-c -fPIC -fpermissive -std=gnu++14
endif

LIBPATH=../../../libs/x64
INCLUDEPATH=../../../includes


INCLUDES=-I/usr/include -I$(INCLUDEPATH) -I${TORCH_PATH}/include -I${TORCH_PATH}/include/torch/csrc/api/include 
LDFLAGS=$(LIBPATH)/libstrategystudio_analytics.a $(LIBPATH)/libstrategystudio.a $(LIBPATH)/libstrategystudio_transport.a $(LIBPATH)/libstrategystudio_marketmodels.a $(LIBPATH)/libstrategystudio_utilities.a $(LIBPATH)/libstrategystudio_flashprotocol.a
LIBRARY=TorchStrategy.so

SOURCES=TorchStrategy.cpp
HEADERS=TorchStrategy.h
 
OBJECTS=$(SOURCES:.cpp=.o)

all: $(HEADERS) $(LIBRARY)

$(LIBRARY) : $(OBJECTS)
	$(CC) -shared -Wl,-soname,$(LIBRARY).1,-R${TORCH_PATH}/lib -o $(LIBRARY) $(OBJECTS) $(LDFLAGS) -L${TORCH_PATH}/lib -ltorch -ltorch_cpu -lc10 
	
.cpp.o: $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf *.o $(LIBRARY)
# Conditional settings based on passed in variables