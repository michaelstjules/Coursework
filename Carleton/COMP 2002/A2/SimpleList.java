/**
*  This is an interface similar to a simplified java.util.List
*  but with some extra get/addFirst & get/addLast methods
*
* (by default, interface methods are all public )
*
* @param <T> the type of data stored in this List
*/

public interface SimpleList<T> {

	//ADDING elements
	//---------------

	//Inserts the given element at the beginning of this list.
	void 	addFirst(T o);

	//Appends the specified element to the end of this list.
	void	addLast(T o);

 	//Inserts the specified element at the specified position in this list.
	//throw IndexOutOfBoundsException if the specified index is out of range (index < 0 || index > size()).
	void 	add(int index, T element);


	//ACCESSING elements
	//------------------

	//Returns the first element in this list. Throw a NoSuchElementException if the SimpleList is empty
	T 	getFirst();

	//Returns the last element in this list. Throw a NoSuchElementException if the SimpleList is empty
	T 	getLast();

	//Returns the element at the specified position in this list.
	//throw IndexOutOfBoundsException if the specified index is out of range (index < 0 || index >= size()).
 	T get(int index);


	//REMOVING elements
	//------------------

	//Removes all of the elements from this list.
	void clear();

	//Removes and returns the element at the specified position in this list.
	//throw IndexOutOfBoundsException if the specified index is out of range (index < 0 || index >= size()).
	 T 	remove(int index);


	//Removes the first occurrence of the specified element in this list.
	//If the list does not contain the element, it is unchanged.
	//returns true if the list contained the specified element, false otherwise
	boolean 	remove(Object o);

	// Removes and returns the first element from this list.
	// throws NoSuchElementException if this queue is empty.
	T 	removeFirst();

	//   Removes and returns the last element from this list.
	//  throws NoSuchElementException if this queue is empty.
	T 	removeLast();


	//MODIFYING elements
	//------------------

	//  Replaces the element at the specified position in this list with the specified element.
	//  throw IndexOutOfBoundsException if the specified index is out of range (index < 0 || index >= size()).
	//	returns the element previously at the specified position.
	 T 	set(int index, T element);


	 //QUERYING the List
	 //-------------------

	//Returns the number of elements in this list.
	int 		size();

	//Returns true if this list contains the specified element, false otherwise
	boolean 	contains(Object o);

	// Returns the index in this list of the first occurrence of the specified element,
	// or -1 if the List does not contain this element.
	int 		indexOf(Object o);

}

class NotYetImplementedException extends RuntimeException{}

//1) add(int index, T element);
//
////throw IndexOutOfBoundsException if the specified index is out of range (index < 0 || index > size()).
//
//[The comments originally said if "... index >= size()"]
//
//This means you can add to the end of a SimpleList like:
//
//list.add( list.size(), obj);
//
//or equivalently for an empty SimpleList:
//
//list.add(0, obj);
//
//
//2) getFirst() and
//3) getLast()
//
////both throw a NoSuchElementException if the SimpleList is empty