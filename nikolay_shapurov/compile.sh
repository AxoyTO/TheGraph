
clang-format -i -style=Chromium main.cpp
clang-format -i -style=Chromium graph.cpp
clang-format -i -style=Chromium graph.hpp
clang-format -i -style=Chromium graphprinter.cpp
clang-format -i -style=Chromium graphprinter.hpp

clang++ main.cpp  graph.cpp graphprinter.cpp -o graph -std=c++17 -Werror

