#include "BloodEntity.h"

BloodEntity::BloodEntity(const int cr/*, const int k, const int j*/) : /*y(k), x(j),*/ age(0), creation(cr) {}
BloodEntity::~BloodEntity(){}
int BloodEntity::getAge() const { return age; }
int BloodEntity::getCreation() const { return creation; }
/*int BloodEntity::getY() const { return y; }
int BloodEntity::getX() const { return x; }

void BloodEntity::setLocation(const int k, const int j)
{
  y = k;
  x = j;
}*/

IncAgeResult BloodEntity::operator~()
{ 
  setAge(getAge()+1); 
  return INTACT;
}

inline void BloodEntity::operator!(){ delete this; }

void BloodEntity::setAge(const int a){ age = a; }
/*void BloodEntity::setY(const int k){ y = k; }
void BloodEntity::setX(const int j){ x = j; }*/

