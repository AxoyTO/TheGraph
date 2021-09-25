all:	
	clang-format -i -style='{BasedOnStyle: Chromium, IndentWidth: 4}' *.cpp
	clang++ main.cpp -o r -std=c++17
	./r
	rm r
	