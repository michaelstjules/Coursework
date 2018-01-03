#include "Blood.h"
#include <iostream>

Blood::Blood() : nodes(new Node<BloodEntity>**[BLOOD_SIZE]) //allocate the rows
{
  for(int k=0; k<BLOOD_SIZE; k++)
    nodes[k] = new Node<BloodEntity>*[BLOOD_SIZE](); //allocate the columns, and initialize to NULL
}

Blood::~Blood()
{
  for(int k=0; k<BLOOD_SIZE; k++)
  {
    for(int j=0; j<BLOOD_SIZE; j++)
    {
      if((*this)(k, j)!=0)
        delete (*this)(k, j)->getValue(); //delete the values in non-null nodes
    }
    delete[] nodes[k]; //delete the columns
  }
  delete[] nodes; //delete the rows
}

Node<BloodEntity> *&Blood::operator()(const int k, const int j) const { return nodes[k][j]; }
/*Node<BloodEntity> *Blood::getNode(const int k, const int j) const { return nodes[k][j]; }
void Blood::setNode(const int k, const int j, Node<BloodEntity> *const u) const { nodes[k][j] = u; }*/

void Blood::swapNodes(const int k, const int j, const int kk, const int jj) const
{
  Node<BloodEntity> *temp = (*this)(k, j);
  (*this)(k, j) = (*this)(kk, jj);;
  //setNode(k, j, getNode(kk, jj));
  (*this)(kk, jj) = temp;
  //setNode(kk, jj, temp);
}