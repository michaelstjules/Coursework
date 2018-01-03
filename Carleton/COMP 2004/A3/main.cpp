#include "header.h"
#include "Inspector.h"

//return a random integer between lower and upper, inclusive
int randomInt(const int lower, const int upper)
{
  //better than simply using mod, since lower order bits aren't so random
  return (int)((double)rand() / ((double)RAND_MAX + 1.0) * (upper - lower + 1)) + lower;
}

int main()
{
  /*comment
  comment
  comment
  comment
  comment
  comment
  comment
  comment
  comment*/
  Inspector inspector(NUM_RBC, NUM_WBC, NUM_BAC);
  inspector.run(NUM_DAYS, P_START_DAY); //this is where the magic happens :D
  return 0;
}