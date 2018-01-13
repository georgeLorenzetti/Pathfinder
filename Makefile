all: pathfinder

pathfinder: pathfinder.o
	g++ pathfinder.o -o pathfinder.bin

pathfinder.o: pathfinder.cpp
	g++ -c pathfinder.cpp