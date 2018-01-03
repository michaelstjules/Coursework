COMPILE = g++ -c -Wall -pedantic
OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

main: header.h $(OBJECTS)
	g++ -o main $(OBJECTS)

main.o: main.cpp
	$(COMPILE) $<

Inspector.o : Inspector.cpp $(wildcard *.h)
	$(COMPILE) $<

%.o: %.cpp %.h header.h
	$(COMPILE) $<

all: main

clean:
	rm -f *.o main