#include "graph_generator.hpp"

using std::cout;
using std::endl;

using Probability = int;

constexpr int GREEN_PROBABILITY = 10, RED_PROBABILITY = 33;

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
}  // namespace

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  graph.add_vertex();  //Нулевая вершина

  //серые грани
  for (Depth current_depth = 0; current_depth <= params_.depth;
       current_depth++) {
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

  //проверка полученной глубины
  if (graph.get_depth_map().size() - 1 < params_.depth)
    std::cout
        << "The resulting depth of the generated graph is less than expected.\n"
        << "Expected depth: " << params_.depth
        << "\nThe resulting depth: " << graph.get_depth_map().size() - 1
        << std::endl;

  const auto& depth_map = graph.get_depth_map();

  //зеленые грани
  for (const auto& vertex_ids : depth_map)
    for (const auto& vertex_id : vertex_ids)
      if (is_lucky(GREEN_PROBABILITY))
        graph.add_edge(vertex_id, vertex_id);

  //желтые грани
  for (Depth depth = 0; depth < depth_map.size() - 1; depth++) {
    const auto& probability =
        100 - get_probability(depth_map.size() - 1, depth);
    const auto& from_vertex_ids = graph.get_vertex_ids_at(depth);
    const auto& to_vertex_ids = graph.get_vertex_ids_at(depth + 1);

    for (const auto& from_vertex_id : from_vertex_ids)
      for (const auto& to_vertex_id : to_vertex_ids)
        if (!graph.is_connected(from_vertex_id, to_vertex_id))
          if (is_lucky(probability))
            graph.add_edge(from_vertex_id, to_vertex_id);
  }

  // красные грани
  for (Depth depth = 0; depth < depth_map.size(); depth++) {
    if (depth + 2 >= depth_map.size())
      break;

    for (const auto& from_vertex_id : depth_map[depth])
      for (const auto& to_vertex_id : depth_map[depth + 2])
        if (is_lucky(RED_PROBABILITY))
          graph.add_edge(from_vertex_id, to_vertex_id);
  }

  return graph;
}
