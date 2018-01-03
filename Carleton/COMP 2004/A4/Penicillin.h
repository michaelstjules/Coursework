#ifndef PENICILLIN_H
#define PENICILLIN_H

#include "header.h"
#include "BloodEntity.h"
#include "Cell.h" //for operator+()

#define P_DESTROY_AGE 1

class Penicillin : public BloodEntity
{
public:
  Penicillin(/*const int k, const int j, */const int cr);
  Type getType(void) const;
  IncAgeResult operator~(void);
  Destroyed operator+(BloodEntity &other);
};

#endif