#include "header.h" //for randomInt()

enum Adjacent {l=0, r, u, d, lu, ld, ru, rd} typedef Adjacent;

template <class T>
class Node
{
  friend class LinkedGrid;
private:
  Node(Node<T> *a0 = 0, Node<T> *a1 = 0, Node<T> *a2 = 0, Node<T> *a3 = 0);
  void setNode(Node<T>* a0 = 0, Node<T>* a1 = 0, Node<T>* a2 = 0, Node<T>* a3 = 0);
  Node<T> *randomAdj(void) const;
  T *x;
  Node<T>* adj[4];
};

template <class T>
Node<T>::Node(Node<T> *a0, Node<T> *a1, Node<T> *a2, Node<T> *a3)
{
  adj[0] = a0;
  adj[1] = a1;
  adj[2] = a2;
  adj[3] = a3;
}

template <class T>
void Node<T>::setNode(Node<T>* a0, Node<T>* a1, Node<T>* a2, Node<T>* a3)
{
  adj[0] = a0;
  adj[1] = a1;
  adj[2] = a2;
  adj[3] = a3;
}

template <class T>
Node<T> *Node<T>::randomAdj(void) const //includes diagonals
{
  switch(randomInt(0, 7))
  {
    case l:
      return adj[l];
    case r:
      return adj[r];
    case u:
      return adj[u];
    case d:
      return adj[d];
    case lu:
      if(adj[l]!=0)
        return adj[l]->adj[u];
      return 0;
    case ld:
      if(adj[l]!=0)
        return adj[l]->adj[d];
      return 0;
    case ru:
      if(adj[r]!=0)
        return adj[r]->adj[u];
      return 0;
    case rd:
      if(adj[r]!=0)
        return adj[r]->adj[d];
      return 0;
  }
}

template <class T>
class LinkedGrid
{
public:
  LinkedGrid(const int rows, const int cols);
  ~LinkedGrid();
  void freeNodes(Node<T> *&u);
  void traverseNodes(void (*function)(Node<T>*&));
  void traverseValues(void (*function)(T&));
private:
  Node<T>* first;
};


template <class T>
LinkedGrid<T>::LinkedGrid(const int rows, const int cols)
{
  //make a 2D array of Node pointers, temporarily
  Node<T> ***nodes = new Node<T>**[rows]; //allocate the rows 
  for(int k=0; k<rows; k++)
  {
    nodes[k] = new Node<T>*[cols]; //allocate the columns
    for(int j=0; j<cols; j++)
    {
      //pointers aren't NULL by default...
      Node<T> *a0 = j>0 ? nodes[k][j-1] : 0;
      Node<T> *a1 = j<cols-1 ? nodes[k][j+1] : 0;
      Node<T> *a2 = k>0 ? nodes[k-1][j] : 0;
      Node<T> *a3 = k<rows-1 ? nodes[k+1][j] : 0;
      nodes[k][j] = new Node<T>(a0, a1, a2, a3); //allocate the actual Nodes
    }
  }
  first = nodes[0][0];
  for(int k=0; k<rows; k++)
    delete[] nodes[k]; //deallocate columns
  delete[] nodes; //deallocate rows
}

template <class T>
LinkedGrid<T>::~LinkedGrid()
{
  freeNodes(first);
}

template <class T>
void LinkedGrid<T>::freeNodes(Node<T> *&u)
{
  if(u==0)
    return;
  //recursively delete nodes to the right and down, before deleting this one
  freeNodes(u->adj[r]);
  freeNodes(u->adj[d]);
  delete u;
  u = 0; //set it to NULL, for the base case, to avoid multiple deletions on same address
}

template <class T>
void LinkedGrid<T>::traverseNodes(void (*function)(Node<T>*&))
{
  for(Node<T> *rowNode=first; rowNode->adj[d]!=0; rowNode=rowNode->adj[d])
  {
    for(Node<T> *u=rowNode; u->adj[r]!=0; u=u->adj[r])
    {
      (*function)(u);
    }
  }
}

template <class T>
void LinkedGrid<T>::traverseValues(void (*function)(T&))
{
  for(Node<T> *rowNode=first; rowNode->adj[d]!=0; rowNode=rowNode->adj[d])
  {
    for(Node<T> *u=rowNode; u->adj[r]!=0; u=u->adj[r])
    {
      (*function)(u->x);
    }
  }
}