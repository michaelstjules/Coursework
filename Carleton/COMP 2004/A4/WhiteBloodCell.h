#ifndef WHITEBLOODCELL_H
#define WHITEBLOODCELL_H

#include "header.h"
#include "DividingCell.h"
#include "RedBloodCell.h" //for operator+()

#define W_START_HEALTH 200
#define W_DESTROY_AGE 12
#define W_DIVIDE_AGE 2

class WhiteBloodCell : public DividingCell
{
public:
  WhiteBloodCell(/*const int k, const int j, */const int cr = 0);
  WhiteBloodCell(const WhiteBloodCell &w);
  Type getType(void) const;
  IncAgeResult operator~(void);
  Destroyed operator+(BloodEntity &other);
  WhiteBloodCell *operator&() const; //cell division
};

#endif