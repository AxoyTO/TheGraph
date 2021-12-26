

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using VertexId = int;

class Vertex {
 public:
  explicit Vertex(VertexId id) : id_(id) {}
  VertexId get_id() const;

 private:
  VertexId id_ = 0;
};
