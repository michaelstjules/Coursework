template <class T>
Node<T>::Node(T *v) : value(v), next(0), prev(0) {}
template <class T>
T *Node<T>::getValue(void) const { return value; }
template <class T>
Node<T> *Node<T>::getNext(void) const { return next; }
template <class T>
Node<T> *Node<T>::getPrev(void) const { return prev; }
template <class T>
void Node<T>::setValue(T *v){ value = v; }
template <class T>
void Node<T>::setNext(Node<T> *u){ next = u; }
template <class T>
void Node<T>::setPrev(Node<T> *u){ prev = u; }