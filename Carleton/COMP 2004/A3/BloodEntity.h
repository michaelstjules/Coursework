#ifndef BLOODENTITY_H
#define BLOODENTITY_H

#include "header.h"

//abstract since there are pure virtual functions

class BloodEntity
{
public:
  BloodEntity(const int cr = 0);
  virtual ~BloodEntity(void);
  int getAge(void) const;
  int getCreation(void) const;
  /* Since you can't use switch statements on typeid, and I'd rather do that
  than several ifs and elses, I use a pure virtual getType() function. */
  virtual Type getType(void) const = 0;
  virtual IncAgeResult incAge(void);
  virtual Destroyed interact(BloodEntity *const other) = 0;
private:
  int age; //in hours
  int creation; //hour of creation
  void setAge(const int a = 0);
};

#endif