BUILD_DIR = ./bin
SRC_DIR = ./src
CC = g++ -std=c++0x 
LIBS = -lpthread -lboost_thread -lnuma
BIGLIB = ./lib/bigLib.so

OBJ = $(BUILD_DIR)/numatest.o
SRC = $(SRC_DIR)/numatest.cc

all: binary binary.debug bigLibBinary

bin/%.o : src/%.cc
	$(CC) -c -o $@ $<

binary: $(BUILD_DIR)/numatest.o
	$(CC) $< $(LIBS) -o $@

binary.debug: $(OBJ)
	$(CC) -g $< $(LIBS) -o $@

bigLibBinary: $(BUILD_DIR)/driver.o
	$(CC) $< $(BIGLIB) -o $@

run : runBigLib

runBigLib :
	LD_LIBRARY_PATH=./lib/:$$LD_LIBRARY_PATH  ./bigLibBinary
clean:
	\rm -f binary binary.debug bin/*

.PHONY: clean run runBigLib
