CXX=g++
SRC=src
BIN=bin
DEBUG=-g
CXXFLAGS+=-W -Wall -Wno-long-long -pedantic -Wno-variadic-macros
CXXFLAGS+=-I.
LDFLAGS+=-lz
LDFLAGS+=-lboost_iostreams
all: seq_lengths seq_lengths2

debug: CXXFLAGS+= -DDEBUG
debug: all

clock: CXXFLAGS+= -DCLOCK
clock: all

seq_lengths: $(SRC)/seq_lengths.cpp ; $(CXX) $(CXXFLAGS) $(SRC)/seq_lengths.cpp -o $(BIN)/seq_lengths $(LDFLAGS)

seq_lengths2: $(SRC)/seq_lengths.cpp ; $(CXX) $(CXXFLAGS) $(SRC)/seq_lengths2.cpp -o $(BIN)/seq_lengths2 $(LDFLAGS)

clean: ; rm -f $(BIN)/seq_lengths.o $(BIN)/seq_lengths

.PHONY: default all clean
