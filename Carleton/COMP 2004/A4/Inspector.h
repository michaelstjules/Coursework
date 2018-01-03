#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "header.h"
#include "BloodEntity.h"
#include "Penicillin.h"
#include "Cell.h"
#include "RedBloodCell.h"
#include "DividingCell.h"
#include "WhiteBloodCell.h"
#include "Bacterium.h"
#include "Node.h"
#include "DoublyLinkedList.h"
#include "Blood.h"

#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <cstdlib> //srand(), rand()
#include <ctime> //time()
using namespace std; 

#define NUM_RBC 500
#define NUM_WBC 100
#define NUM_BAC 50
#define NUM_PEN 200

class Inspector
{
  friend ostream &operator<<(ostream &output, const Inspector &inspector);
  //friend void operator!(BloodEntity &b); //death (removal of the containing Node)
public:
  Inspector(const int b, const int r, const int w);
  int getHour(void) const;
  int getCount(const Type t) const;
  //string toStringCurrent(void) const; //replaced by << operator
  string toString(void) const;
  void run(const int days, const int pStartDay); //call pass every hour on every BloodEntity, adding Penecillin every day starting pStartDay
private:
  //DLL must come before Blood, since Blood deletes values and DLL deletes nodes on destructor call
  DoublyLinkedList<BloodEntity> bloodEntities; 
  Blood blood;
  int hour;
  int counts[4];
  void setHour(const int h);
  void incHour(void); //increment hour
  void setCount(const Type t, const int n = 0);
  void incCount(const Type t); //increment count
  void decCount(const Type t); //decrement count
  void removeBloodEntity(const int k, const int j);
  void addBloodEntity(Type t); 
  void addBloodEntities(const int p = 0, const int b = 0, const int r = 0, const int w = 0);
  void randomUnoccupiedAdjacent(const int k, const int j, int &kk, int &jj) const; 
  void divideCell(const int k, const int j); 
  Node<BloodEntity> *action(const int k, const int j); //returns whatever's at that location after
  bool pass(void); //returns true if there are remaining Cells; false otherwise
};

#endif