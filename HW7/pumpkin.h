#pragma once
#include "init.h"
#include "unicode_chars.h"
#include "my_time.h"

extern uint8_t num_active_pumpkin;

void putPumpkin    (void);
void changeStatus  (void);
bool checkPickUp   (drone_t*);
void printLevel	   (drone_t*);