COMPILE = g++ -c

main:	header.h Point.h Point.o Star.h Star.o Galaxy.h Galaxy.o main.o
	g++ -o main Point.o Star.o Galaxy.o main.o

Point.o:	Point.cpp
	$(COMPILE) Point.cpp

Star.o: Star.cpp
	$(COMPILE) Star.cpp

Galaxy.o:	Galaxy.cpp
	$(COMPILE) Galaxy.cpp

main.o: main.cpp
	$(COMPILE) main.cpp

all:	main

clean:
	rm -f main main.o Point.o Star.o Galaxy.o