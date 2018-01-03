#include "WhiteBloodCell.h"

WhiteBloodCell::WhiteBloodCell(const int cr) : Cell(W_START_HEALTH, cr) {}
Type WhiteBloodCell::getType() const { return WBC; }
IncAgeResult WhiteBloodCell::incAge()
{
  BloodEntity::incAge(); //Cell::incAge() ???
  if(getAge()/24>=W_DESTROY_AGE)
    return DESTROY; 
  return INTACT;
}

Destroyed WhiteBloodCell::interact(BloodEntity *const other)
{
  Cell *otherC = static_cast<Cell*>(other);
  RedBloodCell *otherR = static_cast<RedBloodCell*>(other);
  switch(other->getType())
  {
    case RBC:
      setHealth(getHealth()+50);
      if(!(otherR->setOxygen(otherR->getOxygen()-50)))
        return OTHER;
      return NEITHER;
    case BAC:
      if(getHealth()>otherC->getHealth())
      {
        setHealth(getHealth() - otherC->getHealth());
        //otherC->setHealth(0);
        return OTHER;
      }
      //setHealth(0);
      if(otherC->setHealth(otherC->getHealth() - getHealth()))
        return THIS;
      return BOTH;
    default:
      return NEITHER;
  }
}