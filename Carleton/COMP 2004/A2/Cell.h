#ifndef CELL_H
#define CELL_H

#include "header.h"

#define R_START_HEALTH 200
#define W_START_HEALTH 200
#define B_START_HEALTH 300
#define R_DEATH_AGE 20
#define W_DEATH_AGE 12
#define B_DEATH_AGE 20
#define R_START_OXYGEN 100

enum Type {NA = -1, RBC, WBC, BAC} typedef Type;
enum CellDeath {NEITHER = 0, THIS, C, BOTH} typedef CellDeath; //resulting deaths of cell interactions

class Cell
{
public:
  Type getType(void) const;
  short int getHealth(void) const;
  unsigned short int getAge(void) const;
  char getOxygen(void) const;
  bool incAge(void); //increment age
  Cell(Type t = NA, short int h = 0, char o = 0); //default is empty
  Cell(const Cell &c);
  Cell& operator=(const Cell &c);
  void setCell(Type t = NA, short int h = 0, unsigned short int a = 0, char o = 0); //default is empty
  CellDeath interact(Cell *const c);

private:
  Type type;
  short int health;
  unsigned short int age; //in hours
  char oxygen;
  void setType(Type t = NA);
  void setAge(unsigned short int a = 0);
  void setHealth(short int h = 0);
  void setOxygen(char o = 0);
  void setCell(Cell *const c); //for copying involving "this"
  CellDeath interactRBC(Cell *const c);
  CellDeath interactWBC(Cell *const c);
  CellDeath interactBAC(Cell *const c);
};
#endif