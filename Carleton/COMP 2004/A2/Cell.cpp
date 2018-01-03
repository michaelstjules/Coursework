#include "header.h"
//#include "Cell.h"

Type Cell::getType() const { return type; }
short int Cell::getHealth() const { return health; }
unsigned short int Cell::getAge() const { return age; }
char Cell::getOxygen() const { return oxygen; }

//increment age, return true if the cell aged without dying, false otherwise
bool Cell::incAge()
{ 
  setAge(getAge()+1);
  //check if the cell has died due to age
  switch(getType())
  {
    case RBC:
      if(getAge()/24==R_DEATH_AGE)
      {
        setCell();
        return false;
      }
      break;
    case WBC:
      if(getAge()/24==W_DEATH_AGE)
      {
        setCell();
        return false;
      }
      break;
    case BAC:
      if(getAge()/24==B_DEATH_AGE)
      {
        setCell();
        return false;
      }
      break;
    default:
      break;
  }
  return true;
}

//constructor
Cell::Cell(Type t, short int h, char o): 
type(t), health(h), age(0), oxygen(o){}

//copy constructor
Cell::Cell(const Cell &c)
{
  setCell(c.getType(), c.getHealth(), c.getAge(), c.getOxygen());
}

//assignment
Cell& Cell::operator=(const Cell &c)
{
  setCell(c.getType(), c.getHealth(), c.getAge(), c.getOxygen());
  return *this;
}

void Cell::setCell(Type t, short int h, unsigned short int a, char o)
{
  setType(t);
  setHealth(h);
  setAge(a);
  setOxygen(o);
}

//this cell interacts with c, with results by case
CellDeath Cell::interact(Cell *const c)
{
  switch(c->getType())
  {
    case BAC:
      return interactBAC(c);
    case RBC:
      return interactRBC(c);
    case WBC:
      return interactWBC(c);
    default:
      break;
  }
  return NEITHER;
}

void Cell::setType(Type t){ type = t; }
void Cell::setHealth(short int h){ health = h; }
void Cell::setAge(unsigned short int a){ age = a; }
void Cell::setOxygen(char o){ oxygen = o; }

void Cell::setCell(Cell *const c)
{
  setCell(c->getType(), c->getHealth(), c->getAge(), c->getOxygen());
}

CellDeath Cell::interactRBC(Cell *const c)
{
  switch(this->getType())
  {
    case WBC:
      setHealth(getHealth()+50);
      if(c->getOxygen()<=50) //if c runs out of oxygen
      {
        c->setCell(); //kill c
        return C;
      }
      c->setOxygen(getOxygen()-50);
      break;
    case BAC:
      setHealth(getHealth()+50);
      //move this into c (killing c)
      c->setCell(this);
      setCell();
      return C;
    default:
      break;
  }
  return NEITHER;
}

CellDeath Cell::interactWBC(Cell *const c)
{
  switch(this->getType())
  {
    case RBC:
      c->setHealth(c->getHealth()+50);
      if(getOxygen()<=50) //if this runs out of oxygen
      {
        setCell(); //kill this
        return THIS;
      }
      setOxygen(getOxygen()-50);
      break;
    case BAC:
      if(getHealth()>c->getHealth())
      {
        setHealth(getHealth() - c->getHealth());
        //move this into c (killing c)
        c->setCell(this);
        setCell();
        return C;
      }
      if(c->getHealth()>getHealth())
      {
        c->setHealth(c->getHealth() - getHealth());
        //move c into this (killing this)
        setCell(c);
        c->setCell();
        return THIS;
      }
      //if they have the same health, kill both
      setCell();
      c->setCell();
      return BOTH;
    default:
      break;
  }
  return NEITHER;
}

CellDeath Cell::interactBAC(Cell *const c)
{
  switch(this->getType())
  {
    case RBC:
      c->setHealth(c->getHealth()+50);
      //move c into this (killing this)
      setCell(c);
      c->setCell();
      return THIS;
    case WBC:
      if(c->getHealth()>getHealth())
      {
        c->setHealth(c->getHealth() - getHealth());
        //move c into this (killing this)
        setCell(c);
        c->setCell();
        return THIS;
      }
      if(getHealth()>c->getHealth())
      {
        setHealth(getHealth() - c->getHealth());
        //move this into c (killing c)
        c->setCell(this);
        setCell();
        return C;
      }
      //if they have the same health, kill both
      c->setCell();
      setCell();
      return BOTH;
    default:
      break;
  }
  return NEITHER;
}