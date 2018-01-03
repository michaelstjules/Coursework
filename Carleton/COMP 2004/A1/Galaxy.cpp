#include "header.h"

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