BIN_DIR = $(firstword $(wildcard bin))

CXX = clang++

SRCS = *.cpp

CXXFLAGS =
ifeq ($(OS), Windows_NT)
	CXX = g++
	CXXFLAGS += -static-libgcc -static-libstdc++
endif

LDFLAGS =

main:
ifeq ($(BIN_DIR),)
	mkdir bin
endif
	$(CXX) $(CXXFLAGS) $(SRCS) $(LDFLAGS) -o bin/ptable
