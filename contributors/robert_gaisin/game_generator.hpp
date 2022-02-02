#pragma once
#include "graph_generator.hpp"

namespace uni_course_cpp {

class Game;
class GameGenerator {
 public:
  explicit GameGenerator(const GraphGenerator::Params& params)
      : params_(params){};
  Game generate() const;

 private:
  GraphGenerator::Params params_;
};
}  // namespace uni_course_cpp
