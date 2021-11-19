CXX = clang++
CXXFLAGS = -Wall -std=c++17 -g

all: clean prog

prog:
	$(CXX) $(CXXFLAGS) main.cpp graph.cpp logger.cpp graph_generator.cpp graph_printer.cpp -o prog

clean:
	rm -f prog
