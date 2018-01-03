#include "header.h"

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