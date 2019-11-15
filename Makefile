test: genetica.cpp
	g++ -std=c++17 genetica.cpp -o genetica
	./genetica
test2: test.cpp
	g++ -std=c++17 test.cpp -o test 
	./test
