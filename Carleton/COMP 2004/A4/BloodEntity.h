#ifndef BLOODENTITY_H
#define BLOODENTITY_H

#include "header.h"

//abstract since there are pure virtual functions

class BloodEntity
{
//friend void operator!(BloodEntity &b); //accesses an Inspector method to have this BloodEntity and its Node removed
public:
  BloodEntity(/*const int k, const int j, */const int cr = 0);
  virtual ~BloodEntity(void);
  /*int getY(void) const;
  int getX(void) const;
  void setLocation(const int k, const int j);*/
  int getAge(void) const;
  int getCreation(void) const;
  /* Since you can't use switch statements on typeid, and I'd rather do that
  than several ifs and elses, I use a pure virtual getType() function. */
  virtual Type getType(void) const = 0;
  virtual IncAgeResult operator~(void); //aging
  virtual Destroyed operator+(BloodEntity &other) = 0; //interaction
  void operator!(void);
private:
  //int y, x; //coordinates
  int age; //in hours
  int creation; //hour of creation
  /*void setY(const int k);
  void setX(const int j);
  void setAge(const int a = 0);*/
};

#endif