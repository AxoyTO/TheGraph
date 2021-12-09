CXX = clang++
CXXFLAGS = -Wall -std=c++17 -g -pthread

all: clean prog format

prog:
	$(CXX) $(CXXFLAGS) main.cpp graph.cpp graph_printing.cpp graph_generation_controller.cpp graph_generator.cpp logger.cpp -o prog

format:
	clang-format -i -style=Chromium *.hpp
	clang-format -i -style=Chromium *.cpp

clean:
	rm -f prog
