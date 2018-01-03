Michael St. Jules

A4/
	Bacterium.cpp
	Bacterium.h
	Blood.cpp
	BloodEntity.cpp
	BloodEntity.h
	Blood.h
	Cell.cpp
	Cell.h
	DividingCell.h
	DividingCell.cpp
	DoublyLinkedList.h
	DoublyLinkedListImp.h
	header.h
	Inspector.cpp
	Inspector.h
	main.cpp
	Makefile
	Node.h
	NodeImp.h
	Penicillin.cpp
	Penicillin.h
	readMe.txt
	RedBloodCell.cpp
	RedBloodCell.h
	WhiteBloodCell.cpp
	WhiteBloodCell.h


After compiling with "make", run with "./main". 
Use "make clean" to remove resulting files.

I've only changed the program since A3 enough to accomodate operators.
This meant storing coordinates in BloodEntities for the ! operator (death),
which really just calls removeBloodEntity() with the BloodEntity's coordinates.

While I generally disapprove of overloading the & operator, since it means for 
whatever type it's overloaded, the & will not return the address, all BloodEntities 
are allocated dynamically and their pointers are stored, so I never need to use the 
usual & operator to get one's address. 
Also, operators look bad when pointers are used, since they need to be derefenced first.

While reinterpret_cast should be used sparingly, I do use it, but I only use the 
value returned AFTER checking that it's the correct type (inside my + operator). 
I use reinterpret_cast instead of the others because my + operator takes references, 
not pointers and I cannot use the & operator, since I've overloaded it. >.>

Encapsulation has to be broken to accommodate the ! operator (to access the Inspector).