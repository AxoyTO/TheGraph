#include <sys/stat.h>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include "graph_generation.hpp"
#include "logger.hpp"

std::string get_date_and_time() {
  // char date_and_time[100] = {0};
  std::time_t date_time_t =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  // std::strftime(date_and_time, sizeof(date_and_time), "%d.%m.%Y %T",
  //               std::localtime(&now));
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%d.%m.%Y %T");
  return date_time_string.str();
}

int main() {
  const std::string dafault_filename = "Graph";
  const std::string dafault_fileformat = ".json";

  int graphs_count;  // Количество генерируемых графов.
  std::cout << "Enter the Count of new graphs" << std::endl;
  std::cin >> graphs_count;
  while (graphs_count <= 0) {
    std::cout << "Try to enter a natural number" << std::endl;
    std::cin >> graphs_count;
  }
  uni_cpp_practice::Depth
      depth;  //Глубина графов (int от 0 и до бесконечности).
  std::cout << "Enter the Depth of the graphs" << std::endl;
  std::cin >> depth;
  while (depth < 0) {
    std::cout << "Try to enter a natural number or zero" << std::endl;
    std::cin >> depth;
  }
  int new_vertices_num;  //Количество вершин, генерируемых каждой родительской
                         //вершиной.
  std::cout << "Enter the Number of new vertices from each vertex of the graphs"
            << std::endl;
  std::cin >> new_vertices_num;
  while (new_vertices_num < 0) {
    std::cout << "Try to enter a natural number or zero" << std::endl;
    std::cin >> new_vertices_num;
  }

  try {
    if (!std::filesystem::create_directory("./temp")) {
    }
  } catch (const std::exception& ex) {
    std::cout << ex.what() << "\n";
  }

  auto& logger = uni_cpp_practice::Logger::get_instance();
  logger.set_file("./temp/log.txt");
  for (int i = 0; i < graphs_count; ++i) {
    std::stringstream ss_filename;
    ss_filename << dafault_filename << "_" << std::setfill('0') << std::setw(2)
                << i + 1 << dafault_fileformat;
    std::ofstream file_out;
    file_out.open(ss_filename.str(), std::fstream::out | std::fstream::trunc);
    if (!file_out.is_open()) {
      std::cerr << "Error opening the file " << ss_filename.str();
    } else {
      std::stringstream log_string;
      log_string << get_date_and_time() << ": Graph " << i + 1
                 << ", Generation Started\n";
      const auto graph = uni_cpp_practice::graph_generation::generate_graph(
          depth, new_vertices_num);
      log_string << get_date_and_time() << ": Graph " << i + 1
                 << ", Generation Finished {  \n";
      log_string << "  depth: " << graph.get_depth() << ",\n";
      log_string << "  vertices: " << graph.get_vertex_map().size() << ", [";
      for (uni_cpp_practice::Depth current_depth = 0;
           current_depth <= graph.get_depth(); ++current_depth) {
        log_string << graph.get_vertices_at_depth(current_depth).size();
        if (current_depth != graph.get_depth()) {
          log_string << ", ";
        }
      }
      log_string << "],\n";
      std::vector<std::pair<uni_cpp_practice::Edge::Color, int>>
          count_edges_of_color_map = {
              {uni_cpp_practice::Edge::Color::Gray,
               graph.count_edges_of_color(uni_cpp_practice::Edge::Color::Gray)},
              {uni_cpp_practice::Edge::Color::Green,
               graph.count_edges_of_color(
                   uni_cpp_practice::Edge::Color::Green)},
              {uni_cpp_practice::Edge::Color::Blue,
               graph.count_edges_of_color(uni_cpp_practice::Edge::Color::Blue)},
              {uni_cpp_practice::Edge::Color::Yellow,
               graph.count_edges_of_color(
                   uni_cpp_practice::Edge::Color::Yellow)},
              {uni_cpp_practice::Edge::Color::Red,
               graph.count_edges_of_color(uni_cpp_practice::Edge::Color::Red)}};
      log_string << "  edges: " << graph.get_edge_map().size() << ", {";
      for (int color_index = 0; color_index < count_edges_of_color_map.size();
           ++color_index) {
        log_string << uni_cpp_practice::color_to_string(
                          count_edges_of_color_map[color_index].first)
                   << ": " << count_edges_of_color_map[color_index].second;
        if (color_index + 1 != count_edges_of_color_map.size()) {
          log_string << ", ";
        }
      }
      log_string << "}\n}\n";
      logger.log(log_string.str());

      file_out << graph.to_string();
      file_out.close();
    }
  }

  return 0;
}
