#ifndef REDBLOODCELL_H
#define REDBLOODCELL_H

#include "header.h"
#include "Cell.h"

#define R_START_HEALTH 200
#define R_START_OXYGEN 100
#define R_DESTROY_AGE 20

class RedBloodCell : public Cell
{
public:
  RedBloodCell(/*const int k, const int j, */const int cr = 0);
  Type getType(void) const;
  int getOxygen(void) const;
  bool setOxygen(const int o);
  IncAgeResult operator~(void);
  Destroyed operator+(BloodEntity &other);
private:
  int oxygen;
};

#endif