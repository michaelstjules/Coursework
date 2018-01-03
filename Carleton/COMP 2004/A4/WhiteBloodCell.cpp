#include "WhiteBloodCell.h"

WhiteBloodCell::WhiteBloodCell(/*const int k, const int j, */const int cr) : Cell(/*k, j, */W_START_HEALTH, cr) {}
WhiteBloodCell::WhiteBloodCell(const WhiteBloodCell &w) : DividingCell(w) {}
Type WhiteBloodCell::getType() const { return WBC; }
IncAgeResult WhiteBloodCell::operator~()
{
  BloodEntity::operator~();
  if(getAge()/24>=W_DESTROY_AGE)
    return DESTROY; 
  else if((getAge()%(24*W_DIVIDE_AGE)==0) && (getAge()>0) && (getHealth()>200))
    return DIVIDE; 
  return INTACT;
}

Destroyed WhiteBloodCell::operator+(BloodEntity &other)
{
  Cell &otherC = reinterpret_cast<Cell&>(other);
  RedBloodCell &otherR = reinterpret_cast<RedBloodCell&>(other);
  switch(other.getType())
  {
    case RBC:
      setHealth(getHealth()+50);
      if(!(otherR.setOxygen(otherR.getOxygen()-50)))
        return OTHER;
      return NEITHER;
    case BAC:
      if(getHealth()>otherC.getHealth())
      {
        setHealth(getHealth() - otherC.getHealth());
        return OTHER;
      }
      if(otherC.setHealth(otherC.getHealth() - getHealth()))
        return THIS;
      return BOTH;
    default:
      return NEITHER;
  }
}

//return a pointer to a copy of this
WhiteBloodCell *WhiteBloodCell::operator&() const { return new WhiteBloodCell(*this); }