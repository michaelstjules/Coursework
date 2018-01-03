COMPILE = g++ -c

main:	header.h Cell.h Cell.o Inspector.h Inspector.o main.o
	g++ -o main Cell.o Inspector.o main.o

Cell.o:	Cell.cpp
	$(COMPILE) Cell.cpp

Inspector.o: Inspector.cpp
	$(COMPILE) Inspector.cpp

main.o: main.cpp
	$(COMPILE) main.cpp

all:	main

clean:
	rm -f main main.o Cell.o Inspector.o