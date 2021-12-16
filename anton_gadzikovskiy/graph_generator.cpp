#include "graph_generator.hpp"
#include <random>
#include "graph.hpp"

namespace {

using std::vector;
const double kGreenEdgesProbability = 10.0;
const double kRedEdgesProbability = 33.0;

bool is_generated(double probability) {
  std::random_device rd;
  std::default_random_engine gen(rd());
  std::bernoulli_distribution is_gen(probability * 0.01);
  return is_gen(gen);
}

const uni_cource_cpp::Graph::VertexId& get_random_vertex_id(
    const vector<uni_cource_cpp::Graph::VertexId>& vertices) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_id(0, vertices.size() - 1);
  return vertices[random_id(gen)];
}

void generate_gray_edges(uni_cource_cpp::Graph& graph,
                         int depth,
                         int new_vertices_num) {
  for (int current_depth = 0; current_depth < depth; current_depth++) {
    double step = 100 / (double)depth;
    double probability = 100 - step * current_depth;
    if (current_depth < graph.depth() + 1) {
      //Создаю копию, чтобы не менять объект по которому итерирую
      const auto vertex_ids_on_depth =
          graph.get_vertex_ids_on_depth(current_depth);
      for (const auto& from_vertex_id : vertex_ids_on_depth) {
        for (int k = 0; k < new_vertices_num; k++) {
          if (is_generated(probability)) {
            const auto new_vertex_id = graph.add_vertex();
            graph.add_edge(from_vertex_id, new_vertex_id);
          }
        }
      }
    }
  }
}

void generate_green_edges(uni_cource_cpp::Graph& graph) {
  for (const auto& [vertex_id, vertex] : graph.get_vertices()) {
    if (is_generated(kGreenEdgesProbability)) {
      graph.add_edge(vertex_id, vertex_id);
    }
  }
}

void generate_yellow_edges(uni_cource_cpp::Graph& graph) {
  for (int current_depth = 0; current_depth <= graph.depth(); current_depth++) {
    if (current_depth != graph.depth()) {
      for (const auto& from_vertex_id :
           graph.get_vertex_ids_on_depth(current_depth)) {
        const double probability =
            (double)graph.get_vertex(from_vertex_id).depth *
            (100 / (double)(graph.depth() - 1));
        if (is_generated(probability)) {
          vector<uni_cource_cpp::Graph::VertexId> vertices_on_deeper_depth;
          for (const auto& to_vertex_id :
               graph.get_vertex_ids_on_depth(current_depth + 1)) {
            if (!graph.is_connected(from_vertex_id, to_vertex_id)) {
              vertices_on_deeper_depth.push_back(to_vertex_id);
            }
          }

          if (!vertices_on_deeper_depth.empty()) {
            graph.add_edge(from_vertex_id,
                           get_random_vertex_id(vertices_on_deeper_depth));
          }
        }
      }
    }
  }
}

void generate_red_edges(uni_cource_cpp::Graph& graph) {
  for (int current_depth = 0; current_depth <= graph.depth(); current_depth++) {
    if (current_depth >= graph.depth() - 1) {
      break;
    } else {
      for (const auto& from_vertex_id :
           graph.get_vertex_ids_on_depth(current_depth)) {
        if (is_generated(kRedEdgesProbability)) {
          graph.add_edge(from_vertex_id,
                         get_random_vertex_id(
                             graph.get_vertex_ids_on_depth(current_depth + 2)));
        }
      }
    }
  }
}

}  // namespace

namespace uni_cource_cpp {

Graph GraphGenerator::generate() const {
  auto graph = Graph();

  graph.add_vertex();
  generate_gray_edges(graph, params_.depth(), params_.new_vertices_count());
  generate_green_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);

  return graph;
}

}  // namespace uni_cource_cpp
