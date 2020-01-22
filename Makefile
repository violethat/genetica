%: %.cpp
	g++ -std=c++17 $< -march=native -mtune=native -O3 -o $@
