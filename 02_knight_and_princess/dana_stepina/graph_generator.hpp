#pragma once

#include "graph.hpp"

#include <cmath>
#include <iostream>
#include <random>

using Probability = int;

//Генератор графа
Graph generate_graph(const Depth& depth = 0,
                     const VertexId& vertices_count = 0);
