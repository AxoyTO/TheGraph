CXX = clang++
CXXFLAGS = -Wall -std=c++17 -g -pthread

all: clean graph
	./graph
graph:
	$(CXX) $(CXXFLAGS) main.cpp graph.cpp graph_generator.cpp graph_printer.cpp logger.cpp graph_generation_controller.cpp config.cpp -o graph

format:
	clang-format -i -style=Chromium *.hpp
	clang-format -i -style=Chromium *.cpp

clean:
	rm -f graph
