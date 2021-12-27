#pragma once
#include <string>
#include "graph.hpp"

namespace uni_course_cpp{
namespace printing {
std::string 	print_graph(const uni_course_cpp::Graph & graph);
std::string  generation_started_string(int graph_number);
std::string 	generation_finished_string(int graph_number, const std::string & graph_description);
}
}
