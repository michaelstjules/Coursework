#include <iostream>
#include <typeinfo>

template <class T>
DoublyLinkedList<T>::DoublyLinkedList(): dummy(0), size(0)
{
  dummy.setNext(&dummy);
  dummy.setPrev(&dummy);
}

template <class T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
  Node<T> *curr, *next;
  for(curr=dummy.getNext(); curr!=&dummy; next=curr->getNext(), delete curr, curr=next);
}

template <class T>
int DoublyLinkedList<T>::getSize(void) const { return size; }

template <class T>
bool DoublyLinkedList<T>::isEmpty(void) const { return getSize()==0; }
//return dummy.getNext()==&dummy;


template <class T>
void DoublyLinkedList<T>::addNode(Node<T> *const u)
{
  //assume u is a valid node with valid data
  u->setNext(&dummy);
  u->setPrev(dummy.getPrev());
  dummy.getPrev()->setNext(u);
  dummy.setPrev(u);
  setSize(getSize()+1);
}

template <class T>
void DoublyLinkedList<T>::removeNode(Node<T> *const u)
{
  //assume u is a valid node
  Node<T> *prev = u->getPrev();
  Node<T> *next = u->getNext();
  delete u; //call *u's destructor and free its memory
  prev->setNext(next);
  next->setPrev(prev);
  setSize(getSize()-1);
}

template <class T>
Node<T> *DoublyLinkedList<T>::operator[](const int index)
{
  if((index<0) || (index>=getSize())) //if the index is invalid
    return 0;
  Node<T> *u = &dummy;
  if(index<getSize()/2) //traverse left to right
    for(int i=-1; i<index; i++, u=u->getNext());
  else //traverse right to left
    for(int i=getSize(); i>index; i--, u=u->getPrev());
  return u;
}

template <class T>
void DoublyLinkedList<T>::setSize(const int n){ size = n; }