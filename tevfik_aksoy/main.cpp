#include <array>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "graph.hpp"
#include "graph_generation_controller.hpp"
#include "graph_printer.hpp"
#include "graph_traverser.hpp"
#include "graph_traverser_controller.hpp"
#include "logger.hpp"

using Graph = uni_cpp_practice::Graph;
using Edge = uni_cpp_practice::Edge;
using GraphPrinter = uni_cpp_practice::GraphPrinter;
using GraphGenerator = uni_cpp_practice::GraphGenerator;
using GraphGenerationController = uni_cpp_practice::GraphGenerationController;
using Logger = uni_cpp_practice::Logger;
using GraphTraverser = uni_cpp_practice::GraphTraverser;
using GraphTraverserController = uni_cpp_practice::GraphTraverserController;

const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();

std::string get_date_and_time() {
  std::time_t now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&now), "%d/%m/%Y %T");
  return date_time_string.str();
}

int handle_depth_input() {
  int max_depth = 0;
  std::cout << "Enter max_depth: ";
  do {
    std::cin >> max_depth;
    if (max_depth < 0)
      std::cerr << "Depth can not be negative!\n"
                   "Enter a non-negative max_depth: ";
  } while (max_depth < 0);
  return max_depth;
}

int handle_new_vertices_num_input() {
  int new_vertices_num = 0;
  std::cout << "Enter new_vertices_num: ";
  do {
    std::cin >> new_vertices_num;
    if (new_vertices_num < 0)
      std::cerr << "Number of new vertices created by each vertex can not be "
                   "negative!\nEnter a non-negative new_vertices_num: ";
  } while (new_vertices_num < 0);
  return new_vertices_num;
}

int handle_threads_count_input() {
  int threads_count = 0;
  std::cout << "Enter threads_count: ";
  do {
    std::cin >> threads_count;
    if (threads_count < 0)
      std::cerr << "Count of threads can not be negative!\n"
                   "Enter a non-negative threads_count: ";
  } while (threads_count < 0);
  return threads_count;
}

int handle_graphs_count_input() {
  int graphs_count = 0;
  std::cout << "Enter graphs_count: ";
  do {
    std::cin >> graphs_count;
    if (graphs_count < 0)
      std::cerr << "Count of graphs to be created can not be negative!\n"
                   "Enter a non-negative graphs_count: ";
  } while (graphs_count < 0);
  return graphs_count;
}

void log_start(Logger& logger, const int graph_number) {
  logger.log(get_date_and_time() + ": Graph " + std::to_string(graph_number) +
             ", Generation Started\n");
}

void log_depth(Logger& logger, const Graph& graph) {
  for (int j = 0; j <= graph.depth(); j++) {
    logger.log(std::to_string(graph.get_vertices_in_depth(j).size()));
    if (j != graph.depth())
      logger.log(", ");
  }
}

void log_colors(Logger& logger, const Graph& graph) {
  const std::array<Edge::Color, 5> colors = {
      Edge::Color::Gray, Edge::Color::Green, Edge::Color::Blue,
      Edge::Color::Yellow, Edge::Color::Red};
  for (int i = 0; i < colors.size(); i++) {
    logger.log(uni_cpp_practice::color_to_string(colors[i]) + ": " +
               std::to_string(graph.get_colored_edges(colors[i]).size()));
    if (i + 1 != colors.size())
      logger.log(", ");
  }
}

void log_end(Logger& logger, const Graph& graph, int graph_number) {
  logger.log(get_date_and_time() + ": Graph " + std::to_string(graph_number) +
             ", Generation Finished {  \n");
  logger.log("  depth: " + std::to_string(graph.depth()) + ",\n");
  logger.log("  vertices: " + std::to_string(graph.get_vertices().size()) +
             ", [");
  log_depth(logger, graph);
  logger.log("],\n  edges: " + std::to_string(graph.get_edges().size()) +
             ", {");
  log_colors(logger, graph);
  logger.log("}\n}\n");
}

void log_paths(Logger& logger, const GraphTraverser graph_traverser) {
  const auto shortest_paths = graph_traverser.get_shortest_paths();
  for (const auto& shortest_path : shortest_paths) {
    logger.log("{vertices: [");
    for (int i = 0; i < shortest_path.vertex_ids.size(); i++) {
      logger.log(std::to_string(shortest_path.vertex_ids[i]));
      if (i != shortest_path.vertex_ids.size() - 1)
        logger.log(",");
    }
    logger.log("], distance: " + std::to_string(shortest_path.distance) +
               "},\n");
  }
}

void log_traverser_start(Logger& logger, int graph_number) {
  logger.log(get_date_and_time() + ": Graph " + std::to_string(graph_number) +
             ", Traversal Started\n");
}

void log_traverser_end(Logger& logger,
                       const GraphTraverser& traverser,
                       int graph_number) {
  logger.log(get_date_and_time() + ": Graph " + std::to_string(graph_number) +
             ", Traversal Finished, Paths: [  \n");
  log_paths(logger, traverser);
  logger.log("]\n");
}

void write_to_file(const GraphPrinter& graph_printer,
                   const std::string& filename) {
  std::ofstream jsonfile(filename, std::ios::out);
  if (!jsonfile.is_open())
    throw std::runtime_error("Error while opening the JSON file!");
  jsonfile << graph_printer.print();
  jsonfile.close();
}

void prepare_temp_directory() {
  std::filesystem::create_directory("./temp");
}

void prepare_log_file(Logger& logger) {
  logger.set_file("./temp/log.txt");
}

void traverse_graphs(const std::vector<Graph>& graphs, Logger& logger) {
  // prepare jobs
  // prepare workers
  // start workers
  // wait for all jobs to be done
  // stop workers
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();
  std::atomic<bool> should_terminate = false;
  std::atomic<int> jobs_count = 0;
  // std::mutex traverser_mutex;

  for (int i = 0; i < graphs.size(); i++) {
    jobs.emplace_back([&graphs, &jobs_count, &i]() {
      auto traverser = GraphTraverser(graphs[i]);
      ++jobs_count;
    });
  }

  std::mutex jobs_mutex;
  const auto worker = [&should_terminate, &jobs_mutex, &jobs]() {
    while (true) {
      if (should_terminate) {
        return;
      }
      const auto job_optional = [&jobs_mutex,
                                 &jobs]() -> std::optional<JobCallback> {
        const std::lock_guard lock(jobs_mutex);
        if (jobs.empty()) {
          return std::nullopt;
        }
        const auto first_job = jobs.front();
        jobs.pop_front();
        return first_job;
      }();
      if (job_optional.has_value()) {
        const auto& job = job_optional.value();
        job();
      }
    }
  };

  const auto threads_count = MAX_WORKERS_COUNT;
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_count);

  for (int i = 0; i < threads_count; ++i) {
    threads.push_back(std::thread(worker));
  }

  while (jobs_count < graphs.size()) {
  }

  should_terminate = true;

  for (auto& thread : threads) {
    thread.join();
  }

  /*
  for (int i = 0; i < graphs.size(); i++) {
    auto traverser = GraphTraverser(graphs[i]);
    auto traverser_controller = GraphTraverserController(graphs[i]);
    traverser_controller.traverse(
        [&logger, &i](Graph graph) { log_traverser_start(logger, i); },
        [&logger, &traverser, &i](Graph graph,
                                  std::vector<GraphTraverser::Path> paths) {
          log_traverser_end(logger, traverser, i);
        });
  }
   */
}

int main() {
  const int threads_count = handle_threads_count_input();
  const int graphs_count = handle_graphs_count_input();
  const int max_depth = handle_depth_input();
  const int new_vertices_num = handle_new_vertices_num_input();

  const auto params = GraphGenerator::Params(max_depth, new_vertices_num);
  auto graphs = std::vector<Graph>();
  auto generation_controller =
      GraphGenerationController(threads_count, graphs_count, params);

  auto& logger = Logger::get_instance();

  prepare_temp_directory();
  prepare_log_file(logger);

  graphs.reserve(graphs_count);
  generation_controller.generate(
      [&logger](int index) { log_start(logger, index); },
      [&logger, &graphs](int index, Graph graph) {
        log_end(logger, graph, index);
        graphs.push_back(graph);
        GraphTraverser traverser(graph);
        log_traverser_start(logger, index);
        log_traverser_end(logger, traverser, index);
        const auto graph_printer = GraphPrinter(graph);
        write_to_file(graph_printer,
                      "./temp/graph_" + std::to_string(index) + ".json");
      });

  //traverse_graphs(graphs, logger);

  return 0;
}
