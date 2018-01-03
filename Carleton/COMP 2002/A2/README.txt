Michael St. Jules

A2/
	DoublyLinkedList$1.class
	DoublyLinkedList$Node.class
	DoublyLinkedList.class
	DoublyLinkedList.java
	NotYetImplementedException.class
	README.txt
	SimpleList.class
	SimpleList.java
	TripleArrayDeque.class
	TripleArrayDeque.java

I've included toString() methods for both implementations.

Interestingly, when testing an instance of DoublyLinkedList<Integer> or 
TripleArrayDeque<Integer>, adding or setting an int i equal to an integer
outside of the range of -128 to 127 (the range of a signed char), and then 
using contains(i) will return false (but true within that range). Using int 
g = new Integer(i) and then contains(g) does return true, however. 