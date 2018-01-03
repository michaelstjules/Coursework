#include "header.h"

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