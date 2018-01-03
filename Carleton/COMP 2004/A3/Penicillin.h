#ifndef PENICILLIN_H
#define PENICILLIN_H

#include "header.h"
#include "BloodEntity.h"
#include "Cell.h" //for interact()

#define P_DESTROY_AGE 1

class Penicillin : public BloodEntity
{
public:
  Penicillin(const int cr);
  Type getType(void) const;
  IncAgeResult incAge(void);
  Destroyed interact(BloodEntity *const other);
};

#endif