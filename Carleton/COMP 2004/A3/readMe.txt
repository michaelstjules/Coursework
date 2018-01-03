Michael St. Jules

A3/
	Bacterium.cpp
	Bacterium.h
	Blood.cpp
	BloodEntity.cpp
	BloodEntity.h
	Blood.h
	Cell.cpp
	Cell.h
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
	UML.pdf
	UML.pptx
	WhiteBloodCell.cpp
	WhiteBloodCell.h


After compiling with "make", run with "./main". 
Use "make clean" to remove resulting files.

Both PowerPoint and PDF versions of my UML diagram are included. 

NodeImp.h and DoublyLinkedListImp.h have the implementations for Node.h and DoublyLinkedList.h,
which are template classes (and so their headers include the implementation files). I decided
not to use .cpp files since I don't want them compiled by the Makefile pattern I'm using.

I have the linked list, and it's updated along with the Blood, but it isn't used otherwise.

While the interactions between BloodEntities, in general, and Bacterium division
should realistically be member functions of those BloodEntities or at least the Blood,
it's much simpler and more efficient to implement these in the Inspector, since it must 
also take care of the unnecessary linked list. So, instead of creating and initializing
the Blood separately and passing it to the Inspector, it's also simpler to do just 
initialize the Blood in the Inspector.
Also, while the age of a BloodEntity can realistically be considered its own property,
its hour of creation is relative to something above it (the Inspector's hour), but again,
it's simpler to include it within the BloodEntity. 

I've opted to have the Inspector pass over the 2D array, instead of the linked list,
because I didn't want to store coordinates and wait to delete nodes containing dead cells,
since that's pretty lame, and the instructions said the list should not store dead cells 
(even temporarily?). Then again, passing over 6400 nodes that are mostly empty is pretty lame,
too, especially when you have a linked list.

Prompting for any key is OS-dependant, so I prompt for Enter, specifically.