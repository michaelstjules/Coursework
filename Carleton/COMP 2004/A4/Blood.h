#ifndef BLOOD_H
#define BLOOD_H

#include "header.h"
#include "BloodEntity.h"
#include "Node.h"


class Blood
{
public:
  Blood(void);
  ~Blood(void);
  Node<BloodEntity> *&operator()(const int k, const int j) const;
  /*Node<BloodEntity> *getNode(const int k, const int j) const;
  void setNode(const int k, const int j, Node<BloodEntity> *const u = 0) const;*/
  void swapNodes(const int k, const int j, const int kk, const int jj) const;
private:
  Node<BloodEntity> ***const nodes; //2D array of pointers to Nodes
};

#endif