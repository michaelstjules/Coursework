#include "DividingCell.h"

DividingCell::DividingCell(/*const int k, const int j, */const int h, const int cr) : Cell(k, j, h, cr) {}
//copy everything but age and creation; the age becomes 0 and the creation becomes the sum of the age and the creation of the dividing DividingCell
DividingCell::DividingCell(const DividingCell &c) : Cell(c.getY(), c.getX(), c.getHealth(), c.getCreation()+c.getAge()) {}
DividingCell::~DividingCell(){}