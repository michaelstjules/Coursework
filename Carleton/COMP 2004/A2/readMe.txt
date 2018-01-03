Michael St. Jules

A2/
	A2.cpp
	Cell.cpp
	Cell.h
	header.h
	Inspector.cpp
	Inspector.h
	LinkedGrid.cpp
	main.cpp
	Makefile
	readMe.txt

After compiling with "make", run with "./main". 
Use "make clean" to remove resulting files.


A2.cpp contains all of the other code in a single file.

LinkedGrid.cpp contains an attempt at (the rather awful)
2d list of objects, implemented as a quadruply linked list.
I've got it to compile, but I haven't tested it. 
I also didn't use it in the program, because I didn't want
to ugly it up with it. But, it's here. :)

Prompting for any key is OS-dependant, so I prompt for Enter, specifically.

The Cells' ages are in hours, so I divide by 24 to check its age in days
and kill it on the correct day. Also, cells are aged before they move, 
so if aging should kill one, it will be killed before it moves.
