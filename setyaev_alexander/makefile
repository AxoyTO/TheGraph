TARGET=main
OBJECTS=main.o vertex.o edge.o graph.o
all:$(TARGET)

$(TARGET): $(OBJECTS)
		clang++ -std=c++17 -o $(TARGET) $(OBJECTS)
vertex.o:
		clang++ -std=c++17 -c vertex.cpp
edge.o:
		clang++ -std=c++17 -c edge.cpp
graph.o:
		clang++ -std=c++17 -c graph.cpp
main.o:
		clang++ -std=c++17 -c main.cpp
