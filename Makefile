BUILD_DIR = ./bin
SRC_DIR = ./src
CC = g++ -std=c++0x 
LIBS = -lpthread -lboost_thread -lnuma
BIGLIB = ./lib/bigLib.so

BIGLIBDEPS = $(BUILD_DIR)/main.o $(BUILD_DIR)/driver.o

all: bigLibBinary

bin/%.o : src/%.cc
	$(CC) -c -g -o $@ $<

binary: $(BUILD_DIR)/numatest.o
	$(CC) $< $(LIBS) -o $@

bigLibBinary: $(BIGLIBDEPS)
	$(CC) $(BIGLIBDEPS) $(BIGLIB) $(LIBS) -o $@

run : runBigLib

runBigLib : bigLibBinary
	LD_LIBRARY_PATH=./lib/:$$LD_LIBRARY_PATH  ./bigLibBinary
clean:
	\rm -f binary binary.debug bin/* bigLibBinary

.PHONY: clean run runBigLib
