#include "Bacterium.h"

Bacterium::Bacterium(const int h, const int cr) : Cell(h, cr) {}

/*Bacterium::Bacterium(const BloodEntity &b) : Cell(b.getHealth(), b.getCreation())
{
  setAge(b.getAge());
}*/


Type Bacterium::getType() const { return BAC; }

IncAgeResult Bacterium::incAge()
{
  BloodEntity::incAge();
  if(getAge()/24>=B_DESTROY_AGE)
    return DESTROY; 
  /* >=2 instead of >2, since strict inequality depends on how we're measuring age
  i.e. ageInDays>2 <=> ageInDays>=3, but ageInHours/24>2 <=> ageInHours>=25, different
  times to start dividing. >= is independent of the unit of time */
  else if((getAge()/24>=2) && (getHealth()>200))
    return DIVIDE; 
  return INTACT;
}

Destroyed Bacterium::interact(BloodEntity *const other)
{
  Cell *otherC = static_cast<Cell*>(other);
  switch(other->getType())
  {
    case RBC:
      setHealth(getHealth()+50);
      //otherC->setHealth(0);
      return OTHER;
    case WBC:
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
    case PEN:
      if(!(setHealth(getHealth()-30)))
        return BOTH;
      return OTHER;
    default: //BAC
      return NEITHER; 
  }
  //typeid is dynamic, and type_info is private but static, so you can store its address!
  /*const type_info *const typePtr = &typeid(other);
  if(*typePtr==typeid(Penicillin))
  {
    if(!(setHealth(getHealth()-30)))
      return BOTH;
    return OTHER;
  }
  Cell *otherC = static_cast<Cell*>(other);
  if(*typePtr==typeid(RedBloodCell))
  {
    setHealth(getHealth()+50);
    //otherC->setHealth(0);
    return OTHER;
  }
  if(*typePtr==typeid(WhiteBloodCell))
  {
    if(getHealth()>otherC->getHealth())
    {
      setHealth(getHealth() - otherC->getHealth());
      otherC->setHealth(0);
      return OTHER;
    }
    //setHealth(0);
    if(otherC->setHealth(otherC->getHealth() - getHealth()))
      return THIS;
    return BOTH;
  }
  return NEITHER;*/
}