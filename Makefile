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

all: server client

server:
	$(CXX) $(SRC)/server.c -o $(BUILD)/server

client:
	$(CXX) $(SRC)/client.c -o $(BUILD)/client

run-server:
	$(BUILD)/server

run-client:
	$(BUILD)/client
