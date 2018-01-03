#include "Cell.h"

Cell::Cell(const int h, const int cr) : BloodEntity(cr), health(h) {}
Cell::~Cell(){}
int Cell::getHealth() const { return health; }

bool Cell::setHealth(const int h)
{
  if(h>0)
  {
    health = h;
    return true;
  }
  health = 0;
  return false;
}
