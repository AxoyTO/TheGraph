FLAGS = -std=c++17
all: main

main.cpp: graph.hpp graph_generator.hpp config.hpp
graph.cpp: graph.hpp
graph_generator.cpp: graph_generator.hpp
logger.cpp: logger.hpp
config.cpp: config.hpp

main: main.o graph.o graph_generator.o logger.o config.o
	clang++ main.o graph.o graph_generator.o logger.o config.o -o main $(FLAGS) -lstdc++fs

main.o: main.cpp
	clang++ -c main.cpp $(FLAGS)

graph_generator.o: graph.cpp
	clang++ -c graph_generator.cpp $(FLAGS)

graph.o: graph.cpp
	clang++ -c graph.cpp $(FLAGS)

logger.o: logger.cpp
	clang++ -c logger.cpp $(FLAGS)

config.o: config.cpp
	clang++ -c config.cpp $(FLAGS)

clean:
	rm -f *.o main

run:
	rlwrap ./main