###############################################################################
#
# systems-monitor : Simple Remote Systems Monitoring Tool
#
# Author : Vengateswaran
#
###############################################################################

CXX=gcc
BUILD=./bin
SRC=./src
INCLUDES=./include
SERVER_EXE=rsm-server
CLIENT_EXE=rsm-client
NV_FLAGS=-I/usr/local/cuda/include/ -lnvidia-ml -ldl -L/usr/local/cuda/targets/x86_64-linux/lib/stubs/

all: bin server client

bin:
	mkdir -p $(BUILD)

server:
	$(CXX) $(SRC)/server/*.c -lpthread -I$(INCLUDES) -o $(BUILD)/$(SERVER_EXE)

client:
	$(CXX) $(SRC)/client/*.c -I$(INCLUDES) $(NV_FLAGS) -o $(BUILD)/$(CLIENT_EXE)

run-server:
	$(BUILD)/$(SERVER_EXE)

run-client:
	$(BUILD)/$(CLIENT_EXE)

clean:
	rm -rf bin/*
