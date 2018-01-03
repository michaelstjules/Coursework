#ifndef HEADER_H
#define HEADER_H
#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <ctime>
#include <cstdlib>
#include <cmath>
using namespace std; 

#define MAX_NUM_GALAXIES 1000
#define UNIVERSE_BOUND 599 //0 to 599, which is 600
#define MAX_DISTANCE 50
#include "Point.h"
#include "Star.h"
#include "Galaxy.h"

int randomInt(const int lower, const int upper);
void searchIllum(const int illum, const Galaxy *galaxies, const int &numGalaxies);
#endif