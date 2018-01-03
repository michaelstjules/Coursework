#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include "Node.h"

//this is a pretty minimalistic linked list; nodes are dealt with instead of values

template<class T>
class DoublyLinkedList
{
public:
  DoublyLinkedList(void);
  ~DoublyLinkedList(void);
  int getSize(void) const;
  bool isEmpty(void) const; 
  void addNode(Node<T> *const u);
  void removeNode(Node<T> *const u);
  Node<T> *operator[](const int index); //returns the Node, not its value
private:
  Node<T> dummy; //can still be accessed publicly through other nodes
  int size;
  void setSize(const int n);
};

#include "DoublyLinkedListImp.h"

#endif