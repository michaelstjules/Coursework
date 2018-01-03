#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "header.h"
#include "Cell.h"

#define ENVIRONMENT_SIZE 40
#define NUM_RBC 500
#define NUM_WBC 100
#define NUM_BAC 50

class Inspector
{
public:
  Cell **getEnvironment(void) const;
  unsigned short int getHour(void) const;
  int getCount(const Type t) const;
  Inspector(int b, int r, int w);
  ~Inspector(void);
  void addCells(int b = 0, int r = 0, int w = 0);
  string toString1(void) const;
  string toString2(void) const;
  bool pass(void); //calls moveCell() on cells
  
private:
  Cell **environment;
  unsigned short int hour;
  int cellCounts[3];
  void setEnvironment(Cell **const e);
  void setHour(unsigned short int h);
  void incHour(void); //increment hour
  void setCount(const Type t, const int n = 0);
  void countCells(void);
  void moveCell(const int k, const int j);
};
#endif