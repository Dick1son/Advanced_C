#pragma once

#include "init.h"


void moveDrone(drone_t*);
void moveTrailer(drone_t*);
bool checkDirection(drone_t*, int32_t);
void changeDirection(drone_t*, const int32_t);