#include "graph_generator.hpp"

using std::cout;
using std::endl;

namespace {

Probability get_probability(Depth depth, Depth current_depth) {
  return floor(100 * (depth - current_depth) / depth);
}

bool is_lucky(Probability probability) {
  std::random_device rand;
  std::mt19937 generator(rand());
  std::bernoulli_distribution distribution(probability * 0.01);
  return distribution(generator);
}
GraphGenerator generate_green_edge(Param) 

}  // namespace

//Генератор графа
Graph GraphGenerator::generate() const {
  auto graph = Graph();
  graph.add_vertex();  //Нулевая вершина

  for (Depth current_depth = 0; current_depth <= params_.depth; current_depth++) {
    const auto& probability = get_probability(params_.depth, current_depth);
    const auto& vertex_ids = graph.get_vertex_ids_at(current_depth);

    bool any_new_vertex_generated = false;

    for (const auto& vertex_id : vertex_ids)
      for (int i = 0; i < params_.new_vertices_num; i++)
        if (is_lucky(probability)) {
          const auto& new_vertex = graph.add_vertex();
          graph.add_edge(vertex_id, new_vertex.id);
          any_new_vertex_generated = true;
        }

    if (!any_new_vertex_generated)
      break;
  }

  // if (graph.get_size_depth_map() != depth)
  //   cout<<"The graph depth is less than expected.\n Graph depth -
  //   "<<graph.get_size_depth_map()<<endl;
  return graph;
}
