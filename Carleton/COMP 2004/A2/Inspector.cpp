#include "header.h"
//#include "Cell.h"
//#include "Inspector.h"

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