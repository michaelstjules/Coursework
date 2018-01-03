#ifndef POINT_H
#define POINT_H
#define MAX_DISTANCE 50

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