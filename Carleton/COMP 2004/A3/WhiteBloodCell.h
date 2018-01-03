#ifndef WHITEBLOODCELL_H
#define WHITEBLOODCELL_H

#include "header.h"
#include "Cell.h"
#include "RedBloodCell.h" //for interact()

#define W_START_HEALTH 200
#define W_DESTROY_AGE 12

class WhiteBloodCell : public Cell
{
public:
  WhiteBloodCell(const int cr = 0);
  Type getType(void) const;
  IncAgeResult incAge(void);
  Destroyed interact(BloodEntity *const other);
};

#endif