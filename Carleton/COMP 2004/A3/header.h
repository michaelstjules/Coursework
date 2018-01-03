#ifndef HEADER_H
#define HEADER_H

//day that penecillin drops start being taken
#define P_START_DAY 4 

//number of days to run the simulation
#define NUM_DAYS 30

//height and length of the Blood 2D array
#define BLOOD_SIZE 80

//types representing the concrete derived classes
enum Type{RBC=0, WBC, BAC, PEN} typedef Type;

//resulting destruction of BloodEntity interactions
enum Destroyed {NEITHER = 0, THIS, OTHER, BOTH} typedef Destroyed; 

//result of incAge() on BloodEntities
enum IncAgeResult { INTACT = 0, DESTROY, DIVIDE} typedef IncAgeResult;

//return a random integer between lower and upper, inclusive
int randomInt(const int lower = -1, const int upper = 1);

#endif