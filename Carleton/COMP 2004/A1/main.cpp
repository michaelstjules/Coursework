#include "header.h"

//return a random integer between lower and upper, inclusive
//instead of using srand() in main()
int randomInt(const int lower, const int upper)
{
  long int t = time(NULL)*clock();
  while(time(NULL)*clock()==t); //delay long enough to get a different seed
  srand(time(NULL)*clock());
  //better than simply using mod, since lower order bits aren't so random
  return (int)((double)rand() / ((double)RAND_MAX + 1) * (upper - lower + 1)) + lower;
}


void searchIllum(int illum, const Galaxy *galaxies, const int &numGalaxies)
{
  int i;
  const Galaxy *g = galaxies;
  const Star *s;
  cout << "\n************************************************************\n"
  << "Stars found:" << endl;
  for(i=0; i<numGalaxies; i++, g++)
  {
    for(s=g->getFirstStar(); s->getNextStar()!=NULL; s=s->getNextStar())
    {
      if(s->getIllumination()==illum)
        cout << "Galaxy " << g->getName() << ": " << s->toString() << ", a distance of "
        << g->getLocation()->distance(*(s->getLocation())) << " from the first star" << endl;
    }
  }
  cout << "************************************************************\n" << endl;
}


int main()
{
  Star screen[UNIVERSE_BOUND+1][UNIVERSE_BOUND+1];
  Galaxy galaxies[MAX_NUM_GALAXIES];
  string s;
  int numGalaxies, numStars, illum;
  stringstream ss;
  while(true)
  {
    cout << "\n************************************************************\n"
    << "How many galaxies would you like in your universe?\n"
    << " Enter a number between 0 and " << MAX_NUM_GALAXIES << ", inclusive."
    << "\n************************************************************\n" << endl;
    if((cin >> numGalaxies).fail())
    {
      cout << "\nYour input was not a number. Try again.\n" << endl;
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      continue;
    }
    if((numGalaxies<0) || (numGalaxies>MAX_NUM_GALAXIES))
    {
      cout << "\nYour input of " << numGalaxies << " was out of the range. Try again.\n" << endl;
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      continue;
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int i = 0;
    Galaxy *g = galaxies;
    for(; i<numGalaxies; i++, g++)
    {
      cout << "\n************************************************************\n"
      << "What would you like to name galaxy #" << i+1 << "?" 
      << "\n You may have to hit enter twice."
      << "\n************************************************************\n" << endl;
      getline(cin, s);
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      while(true)
      {
        cout << "\n************************************************************\n"
        << "How many stars should this galaxy have?\n Enter a number strictly greater than 0." 
        << "\n************************************************************\n" << endl;
        if((cin >> numStars).fail())
        {
          cout << "\nYour input was not a number. Try again.\n" << endl;
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          continue;
        }
        if(numStars<1)
        {
          cout << "\nA galaxy cannot have " << numStars << " stars. Try again.\n" << endl;
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          continue;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
      }
      g->setAttributes(screen, s, numStars);
      cout << "\n   " << g->toString() << " was created successfully.\n" << endl;
    }
    break;
  }
  while(true)
  {
    cout << "\n************************************************************\n"
    << "For what illumination degree would you like to search?\n" 
    << " Enter a number between 1 and 100, inclusive.\n"
    << " Or enter \"0\" or \"exit\" to exit." 
    << "\n************************************************************\n" << endl;
    cin >> s;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if((s.compare("0")==0) ||(s.compare("exit")==0))
      exit(0);
    ss.str(s);
    if((ss >> illum).fail())
    {
      cout << "\nYour input of \"" << s << "\" was not a number. Try again.\n" << endl;
      ss.str("");
      ss.clear();
      continue;
    }
    if((illum<1) || (illum>100))
    {
      cout << "\nYour input of " << illum << " was out of the range. Try again.\n" << endl;
      ss.str("");
      ss.clear();
      continue;
    }
    ss.str("");
    ss.clear();
    searchIllum(illum, galaxies, numGalaxies);
  }
  return 0;
}