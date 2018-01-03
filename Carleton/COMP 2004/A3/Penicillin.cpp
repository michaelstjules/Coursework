#include "Penicillin.h"

Penicillin::Penicillin(const int cr) : BloodEntity(cr) {}
Type Penicillin::getType() const { return PEN; }

IncAgeResult Penicillin::incAge()
{
  BloodEntity::incAge();
  if(getAge()/24>=P_DESTROY_AGE)
    return DESTROY;
  return INTACT;
}

//technically, this should never be called, since Penicillin doesn't move
Destroyed Penicillin::interact(BloodEntity *const other)
{ 
  if(other->getType()==BAC)
  {
    Cell *otherC = static_cast<Cell*>(other);
    if(!(otherC->setHealth(otherC->getHealth()-30)))
      return BOTH;
    return THIS;
  }
  return NEITHER;
}