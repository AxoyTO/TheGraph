#include <cstdlib>
#include <fstream>
#include <iostream>
#define constexpr const
constexpr int maxDots = 14;
constexpr int maxVecPerDot = 4;
constexpr int maxEdges = 18;
struct Stliner {
  int maindot;
  int net[maxVecPerDot][2];
};

class vecSetUp {
public:
  void setUp() {
    for (int i = 0; i < maxDots; i++) {
      netWork[i].maindot = i;
      for (int j = 0; j < maxVecPerDot; j++) {
        netWork[i].net[j][0] = -1;
        netWork[i].net[j][1] = -1;
      }
    }
    std::cout << "Setup Tree Completed..." << std::endl;
  }
  bool addFork(int aparm[3]) { // Aparm-> from way to
    for (int i = 0; i < maxDots; i++) {
      if (netWork[aparm[0]].net[i][0] < 0) {
        netWork[aparm[0]].net[i][0] = aparm[1];
        netWork[aparm[0]].net[i][1] = aparm[2];
        break;
      }
    }
    return true;
  }
  bool printJS() {
    std::ofstream graph;
    graph.open("Graphic.json", std::ios::out);
    if (!graph.is_open()) {
      std::cerr << "open file or create error" << std::endl;
    }
    bool beginFlag = true;
    bool seconderay = false;
    // JS begin
    graph << "{\n\t\"vertices\": [\n";
    for (int i = 0; i < maxDots; i++) {
      graph << "{\n\t\t\t\"id\":" << netWork[i].maindot << ","
            << "\n";
      graph << "\t\t\t\"edge_ids\":[";
      beginFlag = true;
      for (int k = 0; k < maxDots; k++) {
        for (int l = 0; l < maxVecPerDot; l++) {
          if (netWork[k].net[l][1] == netWork[i].maindot && k != i) {
            if (beginFlag == true) {
              graph << netWork[k].net[l][0];
              beginFlag = false;
            } else {
              graph << ", " << netWork[k].net[l][0];
            }
          }
        }
      }
      beginFlag = true;
      for (int j = 0; j < maxVecPerDot; j++) {
        if (netWork[i].net[j][0] >= 0) {
          if (beginFlag == true) {
            if (seconderay == true) {
              graph << ", ";
            }
            graph << netWork[i].net[j][0];
            seconderay = true;
            beginFlag = false;
          } else {
            graph << ", " << netWork[i].net[j][0];
          }
        }
      }
      graph << "]\n";
      if (i == (maxDots - 1)) {
        graph << "\t\t}\n";
      } else {
        graph << "\t\t}, ";
      }
    }
    // Edges
    beginFlag = true;
    graph << "\t],\n";
    graph << "\t\"edges\": [\n";
    for (int i = 0; i < maxEdges; i++) {
      for (int j = 0; j < maxDots; j++) {
        for (int k = 0; k < maxVecPerDot; k++) {
          if (i == netWork[j].net[k][0]) { // get Edge ID
            if (beginFlag == false) {
              graph << ",";
            }
            graph << "{\n\t\t\t\"id\": " << i << ",\n";
            graph << "\t\t\t\"vertex_ids\": [" << netWork[j].maindot << ","
                  << netWork[j].net[k][1] << "]\n\t\t}";
            beginFlag = false;
          }
        }
      }
    }
    graph << "\n\t]";
    // JS end
    graph << "\n}\n";
    return true;
  }
  Stliner *netWork;
};
int main() {
  vecSetUp vec;
  vec.netWork = new Stliner[maxDots];
  vec.setUp();
  int vecs[18][3] = {{0, 0, 1},    {0, 1, 2},   {0, 2, 3},   {1, 3, 4},
                     {1, 4, 5},    {1, 5, 6},   {2, 6, 7},   {2, 7, 8},
                     {3, 8, 9},    {4, 9, 10},  {5, 10, 10}, {6, 11, 10},
                     {7, 12, 11},  {8, 13, 11}, {9, 14, 12}, {10, 15, 13},
                     {11, 16, 13}, {12, 17, 13}};
  for (int i = 0; i < 18; i++) {
    if (!vec.addFork(vecs[i])) {
      std::cerr << "Error in data: " << i << std::endl;
    }
    if (!vec.printJS()) {
      std::cerr << "Error in print" << i << std::endl;
    }
  }
  delete[] vec.netWork;
  return 0;
}
