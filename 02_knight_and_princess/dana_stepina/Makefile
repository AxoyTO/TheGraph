CC = clang++
CCFLAGS = -Werror -std=c++17 -g

all: clean main format

main:
	$(CC) $(CCFLAGS) *.cpp -o main

format:
	clang-format -i -style=Chromium *.hpp
	clang-format -i -style=Chromium *.cpp

clean:
	rm -rf *.o main
