CC = clang++
FLAGS = -std=c++17 -Werror -fsanitize=address

all: clean main.o format

main.o:
	$(CC) $(FLAGS) main.cpp graph.cpp graph_generator.cpp graph_printer.cpp logger.cpp -o main.o

format:
	clang-format -i -style=Chromium *.hpp
	clang-format -i -style=Chromium *.cpp

clean:
	rm -f main.o
	rm -f temp/graph*.json
