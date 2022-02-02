#pragma once
#include <string>
#include "graph.hpp"

namespace uni_course_cpp {
namespace printing {
std::string color_to_string(const uni_course_cpp::Edge::Color& color);
std::string print_graph(const uni_course_cpp::Graph& graph);

}  // namespace printing
}  // namespace uni_course_cpp
