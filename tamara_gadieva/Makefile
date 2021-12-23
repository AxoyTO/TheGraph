FLAGS = -g -std=c++17
all: main

main.cpp: graph.hpp graph_generator.hpp config.hpp
graph.cpp: graph.hpp
graph_generator.cpp: graph_generator.hpp
graph_printing.cpp: graph_printing.hpp
logger.cpp: logger.hpp
config.cpp: config.hpp

main: main.o graph.o graph_generator.o logger.o config.o graph_printing.o graph_generation_controller.o
	clang++ main.o graph.o graph_generator.o graph_printing.o logger.o config.o graph_generation_controller.o -o main $(FLAGS) -lpthread

main.o: main.cpp
	clang++ -c main.cpp $(FLAGS)

graph_generator.o: graph_generator.cpp
	clang++ -c graph_generator.cpp $(FLAGS)

graph_generation_controller.o: graph_generation_controller.cpp
	clang++ -c graph_generation_controller.cpp $(FLAGS)

graph_printing.o: graph.cpp
	clang++ -c graph_printing.cpp $(FLAGS)

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