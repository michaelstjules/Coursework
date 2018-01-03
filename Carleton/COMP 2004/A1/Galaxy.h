#ifndef GALAXY_H
#define GALAXY_H
#include "header.h"

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