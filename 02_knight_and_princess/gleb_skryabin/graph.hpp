#include <array>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using idType = int;

class Graph {
 public:
  struct Edge {
    idType id = -1;
    std::pair<idType, idType> vertices;
  };

  struct Vertex {
    idType id = -1;
    std::unordered_set<idType> edges;
  };

  template <std::size_t SIZE>
  explicit Graph(const std::array<Edge, SIZE>& inpEdges) {
    for (auto edge = inpEdges.begin(); edge != inpEdges.end(); edge++) {
      edges.emplace(edge->id, *edge);
    }
    for (auto edge = edges.begin(); edge != edges.end(); edge++) {
      auto vs = edge->second.vertices;
      for (idType vsid : std::array<idType, 2>{vs.first, vs.second}) {
        auto v = vertices.find(vsid);
        if (v == vertices.end()) {
          // add vertex
          Vertex newVertex = {.id = vsid, .edges = {edge->first}};
          vertices.emplace(vsid, newVertex);
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
        idType edgeId = pEdgePair->first;
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
  std::unordered_map<idType, Edge> edges;
  std::unordered_map<idType, Vertex> vertices;
};
