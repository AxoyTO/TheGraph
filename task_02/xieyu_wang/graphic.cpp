#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
constexpr int MAX_VEC = 18;
constexpr int MAX_PTS = 13;
struct Edge {
  int from;
  int edge_id;
  int to;
};
class Vector {
public:
  bool WriteToFile() {
    std::ofstream graph;
    graph.open("Graphic.json", std::ios::out);
    if (!graph.is_open()) {
      return false;
    }
    // graph:
    graph << "{" << std::endl << "\t\"vertices\": [" << std::endl;
    int index = 0;
    // graph loop:
    for (int i = 0; i <= MAX_PTS; ++i) {
      index = 0;
      if (i == 0) {
        graph << "\t\t{" << std::endl;
      } else {
        graph << ",{" << std::endl;
      }
      graph << "\t\t\t\"id\": " << i << ", " << std::endl
            << "\t\t\t\"edge_ids\": [";
      for (auto it : netWork) {
        if (it.from == i || it.to == i) {
          if (index != 0) {
            graph << " ," << it.edge_id;
            index++;
          } else {
            graph << it.edge_id;
            index++;
          }
        }
      }
      graph << "]" << std::endl << "\t\t\t}";
    }
    graph << "\n]," << std::endl;
    // edges:
    graph << std::endl << "\t\"edges\": [" << std::endl;
    for (auto it : netWork) {
      if (it.edge_id != 0) {
        graph << ",{" << std::endl;
      } else {
        graph << "\t\t{" << std::endl;
      }
      graph << "\t\t\t\t\"id\":" << it.edge_id << "," << std::endl;
      graph << "\t\t\t\t\"vertex_ids\": [" << it.from << ", " << it.to << "]"
            << std::endl;
      graph << "\t\t\t}";
    }
    graph << "\n\t]" << std::endl;
    graph << "}" << std::endl;
    graph.close();
    return true;
  }

private:
  static constexpr std::array<Edge, MAX_VEC> netWork = {{{0, 0, 1},
                                                         {0, 1, 2},
                                                         {0, 2, 3},
                                                         {1, 3, 4},
                                                         {1, 4, 5},
                                                         {1, 5, 6},
                                                         {2, 6, 7},
                                                         {2, 7, 8},
                                                         {3, 8, 9},
                                                         {4, 9, 10},
                                                         {5, 10, 10},
                                                         {6, 11, 10},
                                                         {7, 12, 11},
                                                         {8, 13, 11},
                                                         {9, 14, 12},
                                                         {10, 15, 13},
                                                         {11, 16, 13},
                                                         {12, 17, 13}}};
};
int main() {
  Vector vecGen;
  std::cout << "Successfully Create arrays...." << std::endl;
  if (vecGen.WriteToFile()) {
    std::cout << "Successfully create JS file....." << std::endl;
  } else {
    std::cerr << "Error in write to file" << std::endl;
    return 1;
  }
}
