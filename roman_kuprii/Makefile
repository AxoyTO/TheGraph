CXX = clang++
CXXFLAGS = -Wall -std=c++17 -g

all: clean prog format

prog:
	$(CXX) $(CXXFLAGS) main.cpp graph.cpp graph_generator.cpp graph_printer.cpp -o prog

format:
	clang-format -i -style=Chromium *.hpp
	clang-format -i -style=Chromium *.cpp

clean:
	rm -f prog
