
#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  explicit Vertex(VertexId id) : id_(id) {}
  VertexId get_id() const;
  const std::vector<EdgeId>& get_connected_edge_ids() const;
  void add_connected_edge_id(EdgeId id);
  std::string to_string() const;
  int get_depth() const { return depth_; }
  void change_depth(int depth) { depth_ = depth; }

 private:
  VertexId id_ = 0;
  int depth_ = 0;
  std::vector<EdgeId> connected_edge_ids_;
};
