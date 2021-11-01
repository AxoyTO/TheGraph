#include "graph_generate.hpp"

void Graph::add_vertex() {
  vertices_.push_back(Vertex(get_new_vertex_id()));
}

void Graph::add_vertex_inf(const VertexId& cur_vertex_id) {
  this->vertices_[cur_vertex_id].add_edge_id(get_cur_edge_id());
}

void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId to_vertex_id) {
  edges_.push_back(Edge(get_cur_edge_id(), from_vertex_id, to_vertex_id));
  increase_edge_id();
}

std::string Graph::json_string() const {
  std::string result_graph;

  result_graph += "{\n";

  result_graph += "\t\"vertices\": [\n";
  for (int i = 0; i < this->vertices_.size(); i++) {
    result_graph += this->vertices_[i].json_string();
    if (i != this->vertices_.size() - 1)
      result_graph += ",\n";
    else
      result_graph += "\n\t],\n";
  }

  result_graph += "\t\"edges\": [\n";
  for (int i = 0; i < this->edges_.size(); i++) {
    result_graph += this->edges_[i].json_string();
    if (i != this->edges_.size() - 1)
      result_graph += ",\n";
    else
      result_graph += "\n\t]\n";
  }

  result_graph += "}\n";

  return result_graph;
}