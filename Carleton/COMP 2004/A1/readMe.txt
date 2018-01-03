Michael St. Jules

A3/
	A1.cpp
	Galaxy.cpp
	Galaxy.h
	header.h
	main.cpp
	Makefile
	Point.cpp
	Point.h
	readMe.txt
	Star.cpp
	Star.h


A1.cpp contains all of the other code in a single file.

After compiling with "make", run with "./main". 
Use "make clean" to remove resulting files.

getline(cin, s) for a string s requires hitting enter twice
before accepting input, for me, at least. 

Only rudimentary validation of input is included
e.g. 1123adb will be accepted as 1123

I've set the universe/screen/2d array of stars to 600x600 
(#define UNIVERSE_BOUND 599)

Instead of using a seed in main() and passing generated numbers,
I wrote a randomInt(const int, const int) function that
both gets a new seed and generates the numbers, but to avoid
cycles or repeated numbers, it's necessary to use a delay.
This function is called three times for every star, so making
a galaxy with a ton of stars may cause a noticeable delay. 

Also, expect that stars near boundaries will be poorly distributed
(i.e. they will clump around the edges).