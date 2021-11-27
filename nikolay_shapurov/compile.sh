
clang-format -i -style=Chromium main.cpp
clang-format -i -style=Chromium graph.cpp
clang-format -i -style=Chromium graph.hpp
clang-format -i -style=Chromium graph_printer.cpp
clang-format -i -style=Chromium graph_printer.hpp

clang++ main.cpp  graph.cpp graph_printer.cpp -o graph -std=c++17 -Werror

