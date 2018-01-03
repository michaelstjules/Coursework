#include "Penicillin.h"

Penicillin::Penicillin(/*const int k, const int j, */const int cr) : BloodEntity(/*k, j, */cr) {}
Type Penicillin::getType() const { return PEN; }

IncAgeResult Penicillin::operator~()
{
  BloodEntity::operator~();
  if(getAge()/24>=P_DESTROY_AGE)
    return DESTROY;
  return INTACT;
}

//technically, this should never be called, since Penicillin doesn't move
Destroyed Penicillin::operator+(BloodEntity &other)
{ 
  if(other.getType()==BAC)
  {
    Cell &otherC = reinterpret_cast<Cell&>(other);
    if(!(otherC.setHealth(otherC.getHealth()-30)))
      return BOTH;
    return THIS;
  }
  return NEITHER;
}