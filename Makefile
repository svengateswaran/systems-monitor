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

CLIENT_IP="192.168.113.223"

all: bin server client

bin:
	mkdir -p $(BUILD)

server:
	$(CXX) $(SRC)/server/*.c -lpthread -I$(INCLUDES) -o $(BUILD)/server

client:
	$(CXX) $(SRC)/client/*.c -I$(INCLUDES) -o $(BUILD)/client

run-server:
	$(BUILD)/server $(CLIENT_IP)

run-client:
	$(BUILD)/client

clean:
	rm -rf bin/*
