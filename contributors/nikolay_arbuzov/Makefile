all: main
	./main.o

main: *.cpp *.hpp
	g++ *.cpp -o main.o -std=c++17 -pthread
clean:
	rm -rf *.o main
