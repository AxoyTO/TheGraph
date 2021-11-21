
clang-format -i -style=Chromium main.cpp
clang-format -i -style=Chromium graph.cpp
clang-format -i -style=Chromium graph.hpp

clang++ main.cpp  graph.cpp -o graph -std=c++17 -Werror

