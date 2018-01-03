#ifndef CELL_H
#define CELL_H

#include "header.h"
#include "BloodEntity.h"

//abstract since it does not override pure virtual BloodEntity functions

class Cell : public BloodEntity
{
public:
  Cell(const int h, const int cr = 0);
  virtual ~Cell(void);
  int getHealth(void) const;
  bool setHealth(const int h);
private:
  int health;
};

#endif