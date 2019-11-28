CXX = clang++

SRCS = *.cpp

CXXFLAGS =
LDFLAGS =

main:
	$(CXX) $(CXXFLAGS) $(SRCS) $(LDFLAGS) -o ptable
