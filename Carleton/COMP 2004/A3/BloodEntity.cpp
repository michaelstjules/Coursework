#include "BloodEntity.h"

BloodEntity::BloodEntity(const int cr) : age(0), creation(cr) {}
BloodEntity::~BloodEntity(){}
int BloodEntity::getAge() const { return age; }
int BloodEntity::getCreation() const { return creation; }

IncAgeResult BloodEntity::incAge(void)
{ 
  setAge(getAge()+1); 
  return INTACT;
}

void BloodEntity::setAge(const int a){ age = a; }