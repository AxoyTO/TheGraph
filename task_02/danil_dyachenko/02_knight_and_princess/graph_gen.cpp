#include <iostream>
#include <fstream>
#include <vector>

struct Vertex{
    int id;
    std::vector<int> edges;
};

struct Edge{
    int id;
    std::vector<int> vertices;
};

class Graph{
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
public:
    void add_vertices(int n_vertices = 1){
        int n_edges = 1;
        int x;
        vertices.resize(n_vertices);
        for (int i = 0; i < n_vertices; ++i){
            std::cout << "n_edges for " << i << " vertex: ";
            std::cin >> n_edges;
            vertices[i].edges.resize(n_edges);
            vertices[i].id = i;
            for (int j = 0; j < n_edges; ++j){
                std::cin >> x;
                vertices[i].edges[j] = x;
            }
        }
    }
    void print_vertices(void){
        std::cout << "Printing vertices:" << std::endl;
        for (int i = 0; i < vertices.size(); ++i){
            std::cout << i << ": [ ";
            for (int j = 0; j < vertices[i].edges.size(); ++j)
                std::cout << vertices[i].edges[j] << " ";
            std::cout << "]" << std::endl;
        }   
    }
    void add_edges(int n_edges = 1){
        int n_vertices = 2;
        int x, y;
        edges.resize(n_edges);
        for (int i = 0; i < n_edges; ++i){
            edges[i].vertices.resize(n_vertices);
            edges[i].id = i;
            std::cout << i << " edge: " << std::endl;
            for (int j = 0; j < n_vertices; ++j){
                std::cin >> x;
                edges[i].vertices[j] = x;
            }
        }
    }
    void print_edges(void){
        std::cout << "Printing edges:" << std::endl;
        for (int i = 0; i < edges.size(); ++i){
            std::cout << i << ": [ ";
            for (int j = 0; j < edges[i].vertices.size(); ++j)
                std::cout << edges[i].vertices[j] << " ";
            std::cout << "]" << std::endl;
        }   
    }
    void to_json(const char * name){
        std::ofstream my_graph;
        my_graph.open(name, std::ios::out);
        my_graph << "{\n\t\"vertices\": [\n";
        for (int i = 0; i < vertices.size(); ++i){
            my_graph <<  "\t\t{ \"id\": " << i << ", \"edge_ids\": [";
            for (int j = 0; j < vertices[i].edges.size() - 1; ++j){
                my_graph << vertices[i].edges[j] << ", ";
            }
            my_graph << vertices[i].edges[vertices[i].edges.size() - 1] << "]";
            if (i + 1 < vertices.size())
                my_graph << " },\n";
            else
                my_graph << " }\n";
        }
        my_graph << "\t],\n";
        my_graph << "\t\"edges\": [\n";
        for (int i = 0; i < edges.size(); ++i){
            my_graph <<  "\t\t{ \"id\": " << i << ", \"vertex_ids\": [";
            for (int j = 0; j < edges[i].vertices.size() - 1; ++j){
                my_graph << edges[i].vertices[j] << ", ";
            }
            my_graph << edges[i].vertices[edges[i].vertices.size() - 1] << "]";
            if (i + 1 < edges.size())
                my_graph << " },\n";
            else
                my_graph << " }\n";
        }
        my_graph << "\t]\n}\n";
    }
};

int main(void){
    Graph my_graph;
    int n_vertices, n_edges;
    std::cout << "Enter number of vertices and edges: ";
    std::cin >> n_vertices >> n_edges;
    my_graph.add_vertices(n_vertices);
    my_graph.add_edges(n_edges);
    my_graph.to_json("my_graph.json");
    return 0;
}
