#include "header.h"

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