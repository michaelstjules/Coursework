#ifndef DIVIDINGCELL_H
#define DIVIDINGCELL_H

#include "header.h"
#include "Cell.h"

//abstract since it does not override pure virtual BloodEntity functions

class DividingCell : public Cell
{
public:
  DividingCell(/*const int k, const int j, */const int h, const int cr = 0);
  DividingCell(const DividingCell &c); //location will have to be set after
  virtual ~DividingCell(void);
  virtual DividingCell *operator&(void) const = 0; //divide; return a pointer to a copy
  /* since all DividingCells (and all BloodEntities, in general) are dynamically allocated, so
  I'll never need to use & to get the address of a DividingCell, since we're only dealing with pointers.
  Still, overloading the & operator just seems like a bad idea, since it has an expected functionality */
};
  
#endif