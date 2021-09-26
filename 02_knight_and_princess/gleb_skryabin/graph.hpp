#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

class Graph {
 public:
  struct Edge {
    int id;
    std::pair<int, int> vertices;
  };

  struct Vertex {
    int id;
    std::set<int> edges;
  };

  Graph(std::vector<Edge> inpEdges) {
    for (auto edge = inpEdges.begin(); edge != inpEdges.end(); edge++) {
      auto e = edges.find(edge->id);
      if (e == edges.end()) {
        // add edge
        edges.emplace(edge->id, *edge);
      } else {
        // edit edge
        e->second.vertices = edge->vertices;
      }
    }
    for (auto edge = edges.begin(); edge != edges.end(); edge++) {
      auto vs = edge->second.vertices;
      int vsid[] = {vs.first, vs.second};
      for (int i = 0; i < 2; i++) {
        auto v = vertices.find(vsid[i]);
        if (v == vertices.end()) {
          // add vertex
          Vertex newVertex = {vsid[i], {edge->first}};
          vertices.emplace(vsid[i], newVertex);
        } else {
          // edit vertex
          v->second.edges.insert(edge->first);
        }
      }
    }
  }

  void printJSON(char const* filename) {
    std::ofstream json(filename);
    if (json.is_open()) {
      json << "{\n  \"vertices\": [\n    ";
      for (auto pVertexPair = vertices.begin();;) {
        auto v = pVertexPair->second;
        json << "{\n      \"id\": " << v.id;
        json << ",\n      \"edge_ids\": [";

        for (auto pEdgeId = v.edges.begin();;) {
          json << *pEdgeId;
          if (++pEdgeId != v.edges.end()) {
            json << ", ";
          } else {
            break;
          }
        }
        json << "]\n    }";
        if (++pVertexPair != vertices.end()) {
          json << ", ";
        } else {
          break;
        }
      }

      // -------------------------------

      json << "\n  ],\n  \"edges\": [\n    ";
      for (auto pEdgePair = edges.begin();;) {
        int edgeId = pEdgePair->first;
        auto vs = pEdgePair->second.vertices;
        json << "{\n      \"id\": " << edgeId;
        json << ",\n      \"vertex_ids\": [";
        json << vs.first << ", " << vs.second;
        json << "]\n    }";

        if (++pEdgePair != edges.end()) {
          json << ", ";
        } else {
          break;
        }
      }

      json << "\n  ]\n}\n";
      json.close();
      printf("Graph is written to %s\n", filename);
    } else {
      printf("Unable to open file\n");
    }
  }

 private:
  std::map<int, Edge> edges;
  std::map<int, Vertex> vertices;
};
