CXX = clang++
CXXFLAGS = -Wall -std=c++17 -g

all: clean prog format

prog:
	$(CXX) $(CXXFLAGS) main.cpp graph.cpp graph_generating.cpp graph_printing.cpp logger.cpp -o prog

format:
	clang-format -i -style=Chromium *.hpp
	clang-format -i -style=Chromium *.cpp

clean:
	rm -f prog
