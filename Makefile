BUILD_DIR = ./bin
SRC_DIR = ./src
CC = g++ -std=c++0x 
LIBS = -lpthread -lboost_thread -lnuma

OBJ = $(BUILD_DIR)/numatest.o
SRC = $(SRC_DIR)/numatest.cc

all: binary binary.debug

bin/%.o : src/%.cc
	$(CC) -c -o $@ $<

binary: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIBS)

binary.debug: $(SRC) $(DEPS)
	$(CC) -o $@ -g $(SRC) $(LIBS)

clean:
	\rm -f binary binary.debug bin/*

.PHONY: clean
