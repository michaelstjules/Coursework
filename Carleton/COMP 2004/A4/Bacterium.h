#ifndef BACTERIUM_H
#define BACTERIUM_H

#include "header.h"
#include "DividingCell.h"

#define B_START_HEALTH 300
#define B_DESTROY_AGE 5
#define B_DIVIDE_AGE 2

class Bacterium : public DividingCell
{
public:
  Bacterium(/*const int k, const int j, */const int h = B_START_HEALTH, const int cr = 0);
  Bacterium(const Bacterium &b);
  Type getType(void) const;
  IncAgeResult operator~(void);
  Destroyed operator+(BloodEntity &other);
  Bacterium *operator&() const; //cell division
};

#endif