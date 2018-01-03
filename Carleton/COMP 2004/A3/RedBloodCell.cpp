#include "RedBloodCell.h"

RedBloodCell::RedBloodCell(const int cr) : Cell(R_START_HEALTH, cr), oxygen(R_START_OXYGEN) {}
Type RedBloodCell::getType() const { return RBC; }
int RedBloodCell::getOxygen() const { return oxygen; }

bool RedBloodCell::setOxygen(const int o)
{
  if(o>0)
  {
    oxygen = o;
    return true;
  }
  oxygen = 0;
  return false;
}

IncAgeResult RedBloodCell::incAge()
{
  BloodEntity::incAge(); //Cell::incAge() ???
  if(getAge()/24>=R_DESTROY_AGE)
    return DESTROY;
  return INTACT;
}

Destroyed RedBloodCell::interact(BloodEntity *const other)
{
  Cell *otherC = static_cast<Cell*>(other);
  switch(other->getType())
  {
    case WBC:
      otherC->setHealth(otherC->getHealth()+50);
      if(!setOxygen(getOxygen()-50))
        return THIS;
      return NEITHER;
    case BAC:
      otherC->setHealth(otherC->getHealth()+50);
      return THIS;
    default:
      return NEITHER;
  }
}