#ifndef STAR_H
#define STAR_H
#include "Point.h"

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