run: main
	./run

main: compile
	g++ *.o -lmingw32 -lSDL2main -lSDL2 -o run 

compile: *.cpp
	g++ -c *.cpp

cl:
	rm *.o run