#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <ctime>
#include <cstdlib>
#include <cmath>
using namespace std; 

#define MAX_NUM_GALAXIES 1000
#define MAX_DISTANCE 50
#define UNIVERSE_BOUND 599 //0 to 599, which is 600

#ifndef POINT_H
#define POINT_H
class Point
{
public: 
  int getX(void) const;
  int getY(void) const;
  Point(const int i = -1, const int j = -1);
  Point(const Point &p);
  Point& operator=(const Point &p);
  float distance(const Point &p) const;
  int newCoordinate(int (Point::*const getCoor)(void) const) const;
  string toString(void) const;
private:
  int x;
  int y;
  void setX(const int i);
  void setY(const int j);
};
#endif

#ifndef STAR_H
#define STAR_H
class Star
{
public:
  int getId(void) const;
  const Point *getLocation(void) const;
  int getIllumination(void) const;
  const Star *getNextStar(void) const;
  Star();
  Star(const int i, const int x, const int y, Star *nStar);
  Star& operator=(const Star &s);
  void setAttributes(const int i, const int x, const int y, Star *nStar);
  string toString(void) const;
private:
  int id; 
  Point location; 
  int illumination;
  const Star *nextStar;
  void setId(const int i);
  void setLocation(const Point);
  void setIllumination(const int i);
  void setNextStar(const Star * const s);
};
#endif

#ifndef GALAXY_H
#define GALAXY_H
class Galaxy
{
public:
  string getName(void) const;
  int getStarTotal(void) const;
  const Star *getFirstStar(void) const;
  const Point *getLocation(void) const;
  void setAttributes(Star (&screen)[UNIVERSE_BOUND+1][UNIVERSE_BOUND+1], const string s, const int n);
  string toString(void) const;
private:
  string name; 
  int starTotal;
  const Star *firstStar;
  const Point *location;
  void setName(const string s);
  void setStarTotal(const int n);
  void setFirstStar(const Star *s);
  void setLocation(const Point *p);
  void addStars(Star (&screen)[UNIVERSE_BOUND+1][UNIVERSE_BOUND+1], const int &n);
};
#endif

int randomInt(const int lower, const int upper);
void searchIllum(const int illum, const Galaxy *galaxies, const int &numGalaxies);



//Point
int Point::getX() const { return x; }
int Point::getY() const { return y; }
Point::Point(const int i, const int j) : x(i), y(j){}
Point::Point(const Point &p) : x(p.getX()), y(p.getY()){} //copy constructor

Point& Point::operator=(const Point &p)
{
  setX(p.getX());
  setY(p.getY());
  return *this;
}

float Point::distance(const Point &p) const
{
  return sqrt((this->getX() - p.getX())*(this->getX() - p.getX())+(this->getY() - p.getY())*(this->getY() - p.getY()));
}

//return an x or y coordinate within 50 of this point and within the boundaries
int Point::newCoordinate(int (Point::*const getCoor)(void) const) const
{
  const int lower = ((this->*getCoor)()>=MAX_DISTANCE) ? ((this->*getCoor)() - MAX_DISTANCE) : 0;
  const int upper = ((this->*getCoor)()<=UNIVERSE_BOUND - MAX_DISTANCE) ? ((this->*getCoor)() + MAX_DISTANCE) : UNIVERSE_BOUND;
  return randomInt(lower, upper);
}

string Point::toString() const
{ 
  stringstream s;
  s << "(" << x << "," << y << ")"; //formatted like (x,y)
  return s.str();
}

void Point::setX(const int i){ x = i; }
void Point::setY(const int j){ y = j; }



//Star
int Star::getId() const { return id; }
const Point *Star::getLocation() const { return &location; }
int Star::getIllumination() const { return illumination; }
const Star *Star::getNextStar() const { return nextStar; }
Star::Star() { }
Star::Star(const int i, const int x, const int y, Star *nStar) : id(i), location(x, y), nextStar(nStar)
{ 
  setIllumination(randomInt(1, 100));
}

Star& Star::operator=(const Star &s)
{
  setId(s.getId());
  setLocation(*(s.getLocation()));
  setIllumination(s.getIllumination());
  setNextStar(s.getNextStar());
  return *this;
}

void Star::setAttributes(const int i, const int x, const int y, Star *nStar)
{
  setId(i);
  Point p(x, y);
  setLocation(p);
  setIllumination(randomInt(1, 100));
  setNextStar(nStar);
}

string Star::toString() const
{
  stringstream s;
  s << "Star ID " << getId() << " of illumination " << getIllumination() 
  << " at " << getLocation()->Point::toString();
  return s.str();
}

void Star::setId(const int i) { id = i; }
void Star::setLocation(const Point p){ location = p; }
void Star::setIllumination(const int i){ illumination = i; }
void Star::setNextStar(const Star *s){ nextStar = s; }



//Galaxy
string Galaxy::getName() const { return name; }
int Galaxy::getStarTotal() const { return starTotal; }
const Star *Galaxy::getFirstStar() const { return firstStar; }
const Point *Galaxy::getLocation() const { return location; }

void Galaxy::setAttributes(Star (&screen)[UNIVERSE_BOUND+1][UNIVERSE_BOUND+1], const string s, const int n)
{
  setName(s);
  setStarTotal(n);
  addStars(screen, n);
}

string Galaxy::toString() const
{
  stringstream s;
  s << "Galaxy " << getName() << " at location " << getLocation()->toString()
  << " with " << getStarTotal() << " stars"; 
  return s.str();
}

void Galaxy::setName(const string s) { name = s; }
void Galaxy::setStarTotal(const int n) { starTotal = n; }
void Galaxy::setFirstStar(const Star *s){ firstStar = s; }
void Galaxy::setLocation(const Point *p){ location = p; }

void Galaxy::addStars(Star (&screen)[UNIVERSE_BOUND+1][UNIVERSE_BOUND+1], const int &n)
{
  if(n<1)
    return;
  Star *star;
  //store the next coordinates to be used
  int nextX, nextY; 
  int currX = randomInt(0, UNIVERSE_BOUND);
  int currY = randomInt(0, UNIVERSE_BOUND);
  Point p(currX, currY); //first location
  setFirstStar(&(screen[currX][currY]));
  setLocation(screen[currX][currY].getLocation());
  for(int i=0; i<n-1; i++)
  {
    nextX = p.newCoordinate(&Point::getX);
    nextY = p.newCoordinate(&Point::getY);
    star = &(screen[nextX][nextY]); //the next star
    screen[currX][currY].setAttributes(i+1, currX, currY, star);
    if(screen[currX][currY].getNextStar()==NULL)
    {
      cout << "Pointer to next star is NULL after setting, and" ;
      if(star==NULL)
        cout << "NULL";
      else cout << "non-NULL";
      cout << " before. " << endl;
      cout << "Next star's location = (" << nextX << "," << nextY << ")." << endl;
    }
    currX = nextX;
    currY = nextY;
  }
  //last star
  screen[currX][currY].setAttributes(n, currX, currY, NULL);
}




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