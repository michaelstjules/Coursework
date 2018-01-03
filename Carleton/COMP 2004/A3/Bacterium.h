#ifndef BACTERIUM_H
#define BACTERIUM_H

#include "header.h"
#include "Cell.h"

#define B_START_HEALTH 300
#define B_DESTROY_AGE 20
#define B_DIVIDE_AGE 2

class Bacterium : public Cell
{
public:
  Bacterium(const int h = B_START_HEALTH, const int cr = 0);
  //Bacterium(const BloodEntity &b); //default copy constructor is used
  Type getType(void) const;
  IncAgeResult incAge(void);
  Destroyed interact(BloodEntity *const c);
};

#endif