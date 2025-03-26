output: main.o DiningPhilosophersProblem.o
	g++ -std=c++20 main.o DiningPhilosophersProblem.o -o output

main.o: main.cpp
	g++ -std=c++20 -c main.cpp

DiningPhilosophersProblem.o: DiningPhilosophersProblem.cpp
	g++ -std=c++20 -c DiningPhilosophersProblem.cpp

run:
	./output $(PARAM)

clean:
	rm *.o output
