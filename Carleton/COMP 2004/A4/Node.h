#ifndef NODE_H
#define NODE_H

template <class T>
class Node
{
public:
  Node(T *v = 0);
  //use default destructor
  T *getValue(void) const;
  Node<T> *getNext(void) const;
  Node<T> *getPrev(void) const;
  void setValue(T *v);
  void setNext(Node<T> *u);
  void setPrev(Node<T> *u);
private:
  T *value;
  Node<T> *next, *prev;
};

#include "NodeImp.h"

#endif