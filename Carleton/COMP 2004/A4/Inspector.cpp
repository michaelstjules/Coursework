#include "Inspector.h"

Inspector::Inspector(int r, int w, int b): bloodEntities(), blood(), hour(0) //, counts({0, 0, 0, 0}) //<--extended initializer list
{
  setCount(RBC, 0);
  setCount(WBC, 0);
  setCount(BAC, 0);
  setCount(PEN, 0);
  srand(time(0));
  addBloodEntities(0, r, w, b);
}

int Inspector::getHour(void) const { return hour; }
int Inspector::getCount(const Type t) const { return counts[t]; }

void Inspector::removeBloodEntity(const int k, const int j)
{
  if(blood(k, j)!=0) //check if there's anything here at all
  {
    decCount(blood(k, j)->getValue()->getType()); //decrement the count for that nodes type
    !(blood(k, j)->*getValue()); //delete the BloodEntity
    //delete blood(k, j)->getValue(); //delete the BloodEntity
    bloodEntities.removeNode(blood(k, j)); //remove the node from the list (deleting it)
    blood(k, j) = 0; //set the node in that position to null, to prevent attempts to access unallocated heap memory
  }
}

void Inspector::addBloodEntity(const Type t)
{
  //get a random location within the 2D array
  int k = randomInt(0, BLOOD_SIZE-1);
  int j = randomInt(0, BLOOD_SIZE-1);
  removeBloodEntity(k, j); //remove any occupying BloodEntity
  //allocate memory for a new BloodEntity and a new Node to contain it
  BloodEntity *c;
  /* except for the first Cells, prevent new BloodEntities from acting the hour
  they're created by taking the hour that the Inspector will be incrementing to */
  int creationTime = (getHour()==0) ? 0 : getHour()+1; 
  switch(t)
  {
    case RBC:
      c = new RedBloodCell(/*k, j, */creationTime);
      break;
    case WBC:
      c = new WhiteBloodCell(/*k, j, */creationTime);
      break;
    case BAC:
      c = new Bacterium(/*k, j, */creationTime);
      break;
    case PEN:
      c = new Penicillin(/*k, j, */creationTime);
      break;
  }
  blood(k, j) = new Node<BloodEntity>(c);
  bloodEntities.addNode(blood(k, j));
  incCount(t);
}

void Inspector::addBloodEntities(int p, int r, int w, int b)
{
  //add the RedBloodCells
  for(int i=0; i<r; i++)
    addBloodEntity(RBC);
  //add the WhiteBloodCells
  for(int i=0; i<w; i++)
    addBloodEntity(WBC);
  //add the Bacteria
  for(int i=0; i<b; i++)
    addBloodEntity(BAC);
  //add the Penicillin
  for(int i=0; i<p; i++)
    addBloodEntity(PEN);
}

//to print out what the simulation is starting with before running it
/*string Inspector::toStringCurrent(void) const
{
  stringstream s;
  s << "Day " << getHour()/24 + 1 << " at hour " << getHour()%24 << " with\n"
  << "  " << getCount(RBC) << ((getCount(RBC)==1) ? " red blood cell, " : " red blood cells,\n")
  << "  " << getCount(WBC) << ((getCount(WBC)==1) ? " white blood cell, " : " white blood cells,\n")
  << "  " << getCount(BAC) << ((getCount(BAC)==1) ? " bacterium, and " : " bacteria, and\n")
  << "  " << getCount(PEN) << " penecillin " << ((getCount(PEN)==1) ? "drop." : "drops.");
  return s.str();
}*/


string Inspector::toString(void) const
{
  stringstream s;
  s << "Day " << getHour()/24 << " finished with\n"
  << "  " << getCount(RBC) << ((getCount(RBC)==1) ? " red blood cell, " : " red blood cells,\n")
  << "  " << getCount(WBC) << ((getCount(WBC)==1) ? " white blood cell, " : " white blood cells,\n")
  << "  " << getCount(BAC) << ((getCount(BAC)==1) ? " bacterium, and " : " bacteria, and\n")
  << "  " << getCount(PEN) << " penecillin " << ((getCount(PEN)==1) ? "drop." : "drops.");
  return s.str();
}


void Inspector::run(const int days, const int pStartDay)
{
  //setHour(0); //if you want to run again starting with the cells of the previous run
  cout << "\nBEGINNING SIMULATION\nfor " << days << " days, taking penicillin starting day " << pStartDay << ".\n\n"
  << *this << "\n Press Enter to start." << endl;
  cin.ignore(numeric_limits<streamsize>::max(), '\n'); //wait for input
  bool remaining = true; //assume at least one cell exists to begin with
  if(pStartDay<=days) //if penicillin will be taken during the time of the simulation
  {
    while((getHour()<((pStartDay - 1)*24 - 1)) && (remaining)) //before pStartDay
    {
      remaining = pass();
      if(getHour()%24==0) //whenever you finish a day
      {
        cout << toString() << "\n";
        if(remaining)
        {
          cout << " Press Enter to continue to the next day." << endl;
          cin.ignore(numeric_limits<streamsize>::max(), '\n'); //wait for input
        }
      }
    }
    if(getHour()!=((pStartDay - 1)*24 - 1)) //if we stopped early
    {
      cout << "There are no remaining cells." << "\n\nEND OF PROGRAM" << endl;
      return;
    }
  }
  //for the remaining days, while there are remaining cells
  while((getHour()<(days*24 - 1)) && (remaining))
  {
    remaining = pass();
    if(getHour()%24==0) //whenever you finish a day
    {
      cout << toString() << "\n";
      if(remaining)
      {
        cout << " Press Enter to continue to the next day." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //wait for input
        cout << "Taking penicillin for day " << getHour()/24+1 << "." << endl;
        addBloodEntities(NUM_PEN); //take penecillin at the start of the new day
      }
    }
    remaining = ((getCount(RBC)+getCount(WBC)+getCount(BAC))!=0); //update after taking penicillin
  }
  if(getHour()!=(days*24 - 1)) //if we stopped early
    cout << "There are no remaining cells.";
  else
  {
    pass();
    cout << toString();
  }
  cout << "\n\nEND OF PROGRAM" << endl;
}


void Inspector::setHour(const int h){ hour = h; }
void Inspector::incHour(void){ setHour(getHour()+1); }
void Inspector::setCount(const Type t, const int n){ counts[t] = n; }
void Inspector::incCount(const Type t){ setCount(t, getCount(t)+1); }
void Inspector::decCount(const Type t){ setCount(t, getCount(t)-1); }

void Inspector::randomUnoccupiedAdjacent(const int k, const int j, int &kk, int &jj) const
{
  //create a DoublyLinkedList containing the adjacent locations
  int adj[8][2] = {{k-1, j-1}, {k-1, j}, {k-1, j+1}, {k, j-1}, {k, j+1}, {k+1, j-1}, {k+1, j}, {k+1, j+1}};
  Node<int> *adjNodes[8]; //all nodes added to a DLL must be dynamically allocated (since DLL deletes them)
  DoublyLinkedList<int> adjList;
  for(int i=0; i<8; i++)
  {
    //allocate the Nodes separately with new, not new[], since DLL uses delete on individual nodes
    adjNodes[i] = new Node<int>(adj[i]);
    adjList.addNode(adjNodes[i]);
  }
  bool valid = false;
  do
  {
    int newAdj = randomInt(0, adjList.getSize()-1); //the index of a Node to check
    Node<int> *u = adjList[newAdj];
    kk = (u->getValue())[0]; //->*
    jj = (u->getValue())[1]; //->*
    valid = ((kk<BLOOD_SIZE) && (jj<BLOOD_SIZE) && (jj>=0) && (kk>=0) && (blood(kk, jj)==0));
    adjList.removeNode(u);
  } 
  while((!valid) && (!adjList.isEmpty())); //while an adjacent valid position hasn't been found but adjacent positions remain
  if(!valid)
    kk = jj = -1; //flag indicating that no adjacent locations were unoccupied
  //leave scope of DLL, calling its destructor and deleting any remaining Nodes
}

void Inspector::divideCell(const int k, const int j)
{
  Type t = blood(k, j)->getValue()->getType();
  if((t==WBC) || (t==BAC)) //if it's a DividigCell
  {
    int kk, jj;
    randomUnoccupiedAdjacent(k, j, kk, jj);
    if((kk!=-1) && (jj!=-1)) //if an adjacent unoccupied location was found
    {
      incCount(t);
      DividingCell *toCopy = static_cast<DividingCell*>(blood(k, j)->getValue());
      DividingCell *copy = &(*toCopy); //operator& is a pure virtual function, which calls the correct copy constructor
      //copy->setLocation(kk, jj);
      blood(kk, jj) = new Node<BloodEntity>(copy);
      bloodEntities.addNode(blood(kk, jj));
    }
  }
}


//movement after interaction: iff RBC involved, move live one to destroyed's location
Node<BloodEntity> *Inspector::action(const int k, const int j)
{
  Node<BloodEntity> *thisNode = blood(k, j);
  BloodEntity *thisEntity = thisNode->getValue();
  Type tType = thisEntity->getType();
  
  IncAgeResult ageResult = ~(*thisEntity); //increment its age
  if(ageResult==DESTROY) //if the BloodEntity is destroyed due to aging
  {
    //!*thisEntity; //calls the following commented line
    removeBloodEntity(k, j);
    return blood(k, j);
  }
  if(tType==PEN) //if it's Penicillin, we're done, since it doesn't move
    return blood(k, j);
  int kk, jj;
  //generate the location into which to attempt to move
  do
  {
    kk = randomInt(k-1, k+1);
    jj = randomInt(j-1, j+1);
  }
  while((kk==k) && (jj==j)); //reject movement into own location
  
  bool moved = false;
  Destroyed destroyed = NEITHER;
  //ignore any attempt to move past the boundary
  if((jj>=0) && (jj<BLOOD_SIZE) && (kk>=0) && (kk<BLOOD_SIZE))
  {
    Node<BloodEntity> *otherNode = blood(kk, jj);
    if(otherNode==0) //if the space is empty
    {
      //move the cell
      blood.swapNodes(k, j, kk, jj);
      moved = true;
    }
    else //otherwise, interact with whatever's there
    {
      BloodEntity *otherEntity = otherNode->getValue();
      Type oType = otherEntity->getType();
      //make the BloodEntities interact, and remove destroyed BloodEntity or BloodEntities, if any
      destroyed = (*thisEntity)+(*otherEntity); //have them interact
      switch(destroyed)
      {
        case THIS:
          //!*thisEntity; //calls the following commented line
          removeBloodEntity(k, j);
          if((tType==BAC) || (oType==BAC))
          {
            //move other into this
            blood.swapNodes(k, j, kk, jj);
          }
          break;
        case OTHER:
          //!*otherEntity;
          removeBloodEntity(kk, jj);
          if((tType==BAC) || (oType==BAC))
          {
            //move this into other
            blood.swapNodes(k, j, kk, jj);
            moved = true;
          }
          break;
        case BOTH:
          //!*thisEntity;
          //!*thisEntity;
          removeBloodEntity(k, j);
          removeBloodEntity(kk, jj);
          break;
        case NEITHER:
          break;
      }
    }
  }
  if((ageResult!=DIVIDE) || (destroyed==THIS) || (destroyed==BOTH)) //if it wasn't a Bacterium that's supposed to divide
    return blood(k, j);
  /* now ((ageResult==DIVIDE) && (destroyed!=THIS) && (destroyed!=BOTH)) is true
  so this should be a Bacterium that's about to divide
  but it may have moved to the adjacent location */
  int currK = k;
  int currJ = j;
  if(moved) //if it moved
  {
    //its location is the one with which it interacted
    currK = kk;
    currJ = jj;
  }
  divideCell(currK, currJ);
  return blood(k, j);
}


//pass over blood's 2D array, as opposed to the linked list
bool Inspector::pass(void)
{
  int k, j;
  for(k=0; k<BLOOD_SIZE; k++)
  {
    for(j=0; j<BLOOD_SIZE; j++)
    {
      //while the BloodEntity at this location hasn't acted
      /*this ensures that every BloodEntity that hasn't just been created acts exactly once
      during this pass, since Cells could move out of or into this location */
      Node<BloodEntity> *u = blood(k, j);
      while((u!=0) && ((u->getValue()->getAge() + u->getValue()->getCreation())==getHour()))
      {
        u = action(k, j);
        if(u!=0 && u->getValue()==0) cout << k << "," << j << endl;
      }
    }
  }
  incHour();
  //check for remaining cells
  if(getCount(RBC) + getCount(WBC) + getCount(BAC) == 0)
    return false;
  return true;
}


//to print out what the simulation is starting with before running it
ostream &operator<<(ostream &output, const Inspector &inspector)
{
  output << "Day " << inspector.getHour()/24 + 1 << " at hour " << inspector.getHour()%24 << " with\n"
  << "  " << inspector.getCount(RBC) << ((inspector.getCount(RBC)==1) ? " red blood cell, " : " red blood cells,\n")
  << "  " << inspector.getCount(WBC) << ((inspector.getCount(WBC)==1) ? " white blood cell, " : " white blood cells,\n")
  << "  " << inspector.getCount(BAC) << ((inspector.getCount(BAC)==1) ? " bacterium, and " : " bacteria, and\n")
  << "  " << inspector.getCount(PEN) << " penecillin " << ((inspector.getCount(PEN)==1) ? "drop." : "drops.");
  return output;
}