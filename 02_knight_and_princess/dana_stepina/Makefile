CC = clang++
CCFLAGS = -Wall -std=c++17 -g

all: clean main format

main:
	$(CC) $(CCFLAGS) main.cpp graph.cpp graph_printer.cpp -o main

format:
	clang-format -i -style=Chromium *.hpp
	clang-format -i -style=Chromium *.cpp

clean:
	rm -rf *.o main
