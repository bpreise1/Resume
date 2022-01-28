FLAGS = -std=c++17 -Wall -Wextra -pedantic -g -O

all: set1 set2 set3 set4 set5 set6

.PHONY: set1 set2 set3 set4 set5 set6

set1:
	g++ test-kec.cpp -o test-kec $(FLAGS)

set2:
	g++ test.cpp -o test $(FLAGS)

set3:
	g++ minimal.cpp -o minimal $(FLAGS)

set4:
	g++ morseex.cpp -o morseex $(FLAGS)

set5:
	g++ -O test-scaling.cpp -o test-scaling $(FLAGS)

set6:
	g++ MyTest.cpp -o MyTest $(FLAGS)

clean:
	rm test-kec test minimal morseex test-scaling
