main:	header.h tools.o userIO.o main.o
	gcc -o main main.o tools.o userIO.o

tools.o:	tools.c
	gcc -c tools.c

userIO.o:	userIO.c
	gcc -c userIO.c

main.o:	main.c
	gcc -c main.c

all:	main

clean:
	rm -f main main.o tools.o userIO.o


# $* --	file name part of current dependent (minus .suffix).
# $@ --	full target name of current target.
# $< -- .c file of target.