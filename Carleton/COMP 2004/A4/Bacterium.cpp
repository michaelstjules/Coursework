#include "Bacterium.h"

Bacterium::Bacterium(/*const int k, const int j, */const int h, const int cr) : DividingCell(/*k,j, */h, cr) {}
Bacterium::Bacterium(const Bacterium &b) : DividingCell(b) {}
Type Bacterium::getType() const { return BAC; }

IncAgeResult Bacterium::operator~()
{
  BloodEntity::operator~();
  if(getAge()/24>=B_DESTROY_AGE)
    return DESTROY; 
  else if((getAge()%(24*B_DIVIDE_AGE)==0) && (getAge()>0) && (getHealth()>200))
    return DIVIDE; 
  return INTACT;
}

Destroyed Bacterium::operator+(BloodEntity &other)
{
  Cell &otherC = reinterpret_cast<Cell&>(other);
  switch(other.getType())
  {
    case RBC:
      setHealth(getHealth()+50);
      return OTHER;
    case WBC:
      if(getHealth()>otherC.getHealth())
      {
        setHealth(getHealth() - otherC.getHealth());
        return OTHER;
      }
      if(otherC.setHealth(otherC.getHealth() - getHealth()))
        return THIS;
      return BOTH;
    case PEN:
      if(!(setHealth(getHealth()-30)))
        return BOTH;
      return OTHER;
    default: //BAC
      return NEITHER; 
  }
}

//return a pointer to a copy of this
Bacterium *Bacterium::operator&() const { return new Bacterium(*this); }