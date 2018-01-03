#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <cstdlib> //srand(), rand(), NULL
#include <ctime> //time()
using namespace std; 




#ifndef CELL_H
#define CELL_H

#define R_START_HEALTH 200
#define W_START_HEALTH 200
#define B_START_HEALTH 300
#define R_DEATH_AGE 20
#define W_DEATH_AGE 12
#define B_DEATH_AGE 20
#define R_START_OXYGEN 100

enum Type {NA = -1, RBC, WBC, BAC} typedef Type;
enum CellDeath {NEITHER = 0, THIS, C, BOTH} typedef CellDeath; //resulting deaths of cell interactions

class Cell
{
public:
  Type getType(void) const;
  short int getHealth(void) const;
  unsigned short int getAge(void) const;
  char getOxygen(void) const;
  bool incAge(void); //increment age
  Cell(Type t = NA, short int h = 0, char o = 0); //default is empty
  Cell(const Cell &c);
  Cell& operator=(const Cell &c);
  void setCell(Type t = NA, short int h = 0, unsigned short int a = 0, char o = 0); //default is empty
  CellDeath interact(Cell *const c);

private:
  Type type;
  short int health;
  unsigned short int age; //in hours
  char oxygen;
  void setType(Type t = NA);
  void setAge(unsigned short int a = 0);
  void setHealth(short int h = 0);
  void setOxygen(char o = 0);
  void setCell(Cell *const c); //for copying involving "this"
  CellDeath interactRBC(Cell *const c);
  CellDeath interactWBC(Cell *const c);
  CellDeath interactBAC(Cell *const c);
};
#endif



#ifndef INSPECTOR_H
#define INSPECTOR_H

#define ENVIRONMENT_SIZE 40
#define NUM_RBC 500
#define NUM_WBC 100
#define NUM_BAC 50

class Inspector
{
public:
  Cell **getEnvironment(void) const;
  unsigned short int getHour(void) const;
  int getCount(const Type t) const;
  Inspector(int b, int r, int w);
  ~Inspector(void);
  void addCells(int b = 0, int r = 0, int w = 0);
  string toString1(void) const;
  string toString2(void) const;
  bool pass(void); //calls moveCell() on cells
  
private:
  Cell **environment;
  unsigned short int hour;
  int cellCounts[3];
  void setEnvironment(Cell **const e);
  void setHour(unsigned short int h);
  void incHour(void); //increment hour
  void setCount(const Type t, const int n = 0);
  void countCells(void);
  void moveCell(const int k, const int j);
};
#endif




int randomInt(const int lower = -1, const int upper = 1);





//Cell
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



//Inspector
Cell **Inspector::getEnvironment(void) const { return environment; }
unsigned short int Inspector::getHour(void) const { return hour; }
int Inspector::getCount(const Type t) const { return cellCounts[t]; }

Inspector::Inspector(int r, int w, int b): hour(0)
{
  setEnvironment(new Cell*[ENVIRONMENT_SIZE]); //allocate the rows
  for(int k=0; k<ENVIRONMENT_SIZE; k++)
    getEnvironment()[k] = new Cell[ENVIRONMENT_SIZE]; //allocate the columns
  addCells(r, w, b);
}

Inspector::~Inspector()
{
  for(int k=0; k<ENVIRONMENT_SIZE; k++)
    delete[] getEnvironment()[k]; //free the columns
  delete[] getEnvironment(); //free the rows
}

void Inspector::addCells(int r, int w, int b)
{
  int i;
  for(i=0; i<r; i++)
  {
    getEnvironment()[randomInt(0, ENVIRONMENT_SIZE-1)][randomInt(0, ENVIRONMENT_SIZE-1)].setCell(RBC, R_START_HEALTH, 0, R_START_OXYGEN);
  }
  for(i=0; i<w; i++)
  {
    getEnvironment()[randomInt(0, ENVIRONMENT_SIZE-1)][randomInt(0, ENVIRONMENT_SIZE-1)].setCell(WBC, W_START_HEALTH);
  }
  for(i=0; i<b; i++)
  {
    getEnvironment()[randomInt(0, ENVIRONMENT_SIZE-1)][randomInt(0, ENVIRONMENT_SIZE-1)].setCell(BAC, B_START_HEALTH);
  }
  countCells(); /* because cells will replace one another,
  so r, w and b are overestimates of the change in cell counts */
}

string Inspector::toString1(void) const
{
  stringstream s;
  s << "Day " << getHour()/24 + 1 << " at hour " << getHour()%24 << " with "
  << getCount(RBC) << ((getCount(RBC)==1) ? " red blood cell, " : " red blood cells, ")
  << getCount(WBC) << ((getCount(WBC)==1) ? " white blood cell, " : " white blood cells, and ")
  << getCount(BAC) << ((getCount(BAC)==1) ? " bacterium." : " bacteria.");
  return s.str();
}

string Inspector::toString2(void) const
{
  stringstream s;
  s << "Day " << getHour()/24 << " finished with "
  << getCount(RBC) << ((getCount(RBC)==1) ? " red blood cell, " : " red blood cells, ")
  << getCount(WBC) << ((getCount(WBC)==1) ? " white blood cell, " : " white blood cells, and ")
  << getCount(BAC) << ((getCount(BAC)==1) ? " bacterium." : " bacteria.");
  return s.str();
}

bool Inspector::pass(void)
{
  int k, j;
  for(k=0; k<ENVIRONMENT_SIZE; k++)
  {
    for(j=0; j<ENVIRONMENT_SIZE; j++)
    {
      //while the cell at this location hasn't moved, move it
      //this ensures that no cell moves twice and that no cell is missed during a pass
      //since cells could move out of or into this location
      while((getEnvironment()[k][j].getType()!=NA) && (getEnvironment()[k][j].getAge()==getHour()))
        moveCell(k, j);
    }
  }
  incHour();
  //check for remaining cells
  if(getCount(RBC) + getCount(WBC) + getCount(BAC) == 0)
    return false;
  return true;
}

void Inspector::setEnvironment(Cell **const e){ environment = e; }
void Inspector::setHour(unsigned short int h){ hour = h; }
void Inspector::incHour(void){ setHour(getHour()+1); } //increment hour

void Inspector::setCount(const Type t, const int n)
{ 
  cellCounts[t] = n; 
  //check if the count dropped to 0
  /* while it may not be standard to put output in the actual class,
  although an "inspector" may be an exception, this is much cleaner than
  keeping a boolean for each Cell type describing whether the count has 
  hit 0 yet, for output only on the first time it happens */
  if(n==0)
  {
    string typeString;
    switch(t)
    {
      case RBC:
        typeString = "red blood cells";
        break;
      case WBC:
        typeString = "white blood cells";
        break;
      case BAC:
        typeString = "bacteria";
        break;
      default:
        return;
    }
    cout << "There are no remaining " << typeString << " live in the blood." << endl;
  }
}

/* it's necessary to count the cells after adding,
because cells can replace each other */
void Inspector::countCells()
{
  int r = 0;
  int w = 0;
  int b = 0;
  //for each cell, count the number of occurrences of each type
  for(int k=0; k<ENVIRONMENT_SIZE; k++)
  {
    for(int j=0; j<ENVIRONMENT_SIZE; j++)
    {
      switch(environment[k][j].getType())
      {
          case RBC:
            r++;
            break;
          case WBC:
            w++;
            break;
          case BAC:
            b++;
            break;
          default:
            break;
      }
    }
  }
  setCount(RBC, r);
  setCount(WBC, w);
  setCount(BAC, b);
}


void Inspector::moveCell(const int k, const int j)
{
  Cell *move = &(getEnvironment()[k][j]);
  Type mType = move->getType();
  
  if(!move->incAge()) //the cell ages, and if that kills it
  {
    setCount(mType, getCount(mType)-1);
    return;
  }
  
  //generate the location into which to attempt to move
  int kk, jj;
  do
  {
    kk = randomInt(k-1, k+1);
    jj = randomInt(j-1, j+1);
  }
  while((kk==k) && (jj==j)); //reject movement into own location
  
  //ignore any attempt to move past the boundary
  if((jj<0) || (jj>=ENVIRONMENT_SIZE) || (kk<0) || (kk>=ENVIRONMENT_SIZE))
    return;

  Cell *occup = &(getEnvironment()[kk][jj]);
  Type oType = occup->getType();
    
  if(oType==NA) //if the space is empty
  {
    //put move in occup's location
    *occup = *move; //assign move's data to occup
    move->setCell(); //clear move's previous location
    return;
  }
  
  //make the two cells interact and check for resulting deaths, and handle cell counts
  switch(move->interact(occup))
  {
    case THIS:
      setCount(mType, getCount(mType)-1);
      break;
    case C:
      setCount(oType, getCount(oType)-1);
      break;
    case BOTH:
      setCount(mType, getCount(mType)-1);
      setCount(oType, getCount(oType)-1);
      break;
    default:
      break;
  }
}





//return a random integer between lower and upper, inclusive
int randomInt(const int lower, const int upper)
{
  //better than simply using mod, since lower order bits aren't so random
  return (int)((double)rand() / ((double)RAND_MAX + 1.0) * (upper - lower + 1)) + lower;
}


int main()
{
  srand(time(NULL));
  Inspector inspector(NUM_RBC, NUM_WBC, NUM_BAC);
  
  cout << "\nBEGINNING SIMULATION\n\n"
  << inspector.toString1() << "\n Press Enter to start the first day by the hour." << endl;
  cin.ignore(numeric_limits<streamsize>::max(), '\n'); //wait for input
  
  bool remaining = true; //assume at least one cell exists to begin with
  //for the first day, while there are remaining cells
  while((inspector.getHour()<23) && (remaining))
  {
    remaining = inspector.pass();
    cout << inspector.toString1() << "\n Press Enter to continue to the next hour." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //wait for input
  }
  if(!remaining) //if the loop was cut short since all the cells died
  {
    cout << "There are no remaining cells.\n\nEND OF PROGRAM" << endl;
    exit(0);
  }
  //for 6 more days, while there are remaining cells
  while((inspector.getHour()<7*24-1) && (remaining))
  {
    remaining = inspector.pass();
    if(inspector.getHour()%24==0) //whenever you finish a day
    {
      cout << inspector.toString2() << "\n Press Enter to continue to the next day." << endl;
      cin.ignore(numeric_limits<streamsize>::max(), '\n'); //wait for input
    }
  }
  if(inspector.getHour()!=7*24-1) //if we stopped early
    cout << "There are no remaining cells.";
  else
  {
    inspector.pass();
    cout << inspector.toString2();
  }
  cout << "\n\nEND OF PROGRAM" << endl;
  return 0;
}