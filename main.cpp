#include <filesystem>
#include "config.hpp"
#include "fstream"
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_json_printing.hpp"
#include "graph_printing.hpp"
#include "iostream"
#include "logger.hpp"
using std::string;
void prepareTempDirectory() {
  std::filesystem::create_directory(uni_course_cpp::config::kTempDirectoryPath);
}
int intInput(string inputMessage) {
  int input = -1;
  std::cout << inputMessage;
  std::cin >> input;
  while ((std::cin.fail() || input < 0)) {
    std::cout << ((input < 0) ? ("Error: input cannot be <0")
                              : "Error: input type != int")
              << "\n";
    std::cin.clear();
    std::cin.ignore(256, '\n');
    std::cout << "Input " << inputMessage;
    std::cin >> input;
  }
  return input;
};
void writeToFile(string const& output, string const& filename) {
  std::ofstream fileToWrite(uni_course_cpp::config::kTempDirectoryPath +
                            filename);
  fileToWrite << output;
}
string generationStartedString(int i) {
  return ("Graph " + std::to_string(i) + ", generation started");
}
string generationFinishedString(int i, std::string string) {
  return ("Graph " + std::to_string(i) + ", generation finished" + string);
}
int main() {
  int const depth = intInput("depth: ");
  int const newVerticesNum = intInput("new_vertex_num: ");
  int const graphsCount = intInput("graphs_count: ");
  prepareTempDirectory();
  auto const params =
      uni_course_cpp::GraphGenerator::Params(depth, newVerticesNum);
  auto const generator = uni_course_cpp::GraphGenerator(params);
  auto& logger = uni_course_cpp::Logger::getLogger();
  for (int i = 0; i < graphsCount; i++) {
    logger.log(generationStartedString(i));
    auto const graph = generator.generate();
    auto const graphPrinter = uni_course_cpp::GraphJsonPrinter(graph);
    auto const graphJson = graphPrinter.print();
    auto const graphDescription =
        uni_course_cpp::GraphPrinter::printGraph(graph);
    logger.log(generationFinishedString(i, graphDescription));
    writeToFile(graphJson, "graph_" + std::to_string(i) + ".json");
  }
  return 0;
}
