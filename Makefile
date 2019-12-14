CXX = clang++

SRCS = *.cpp

CXXFLAGS =
ifeq ($(OS), Windows_NT)
	CXXFLAGS += -static-libgcc -static-libstdc++
endif

LDFLAGS =

main:
	if [ ! -d "bin" ]; then mkdir bin; fi
	$(CXX) $(CXXFLAGS) $(SRCS) $(LDFLAGS) -o bin/ptable
