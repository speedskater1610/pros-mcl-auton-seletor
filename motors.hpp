#pragma once
#include "api.h"
#include "lemlib/api.hpp"

// intake functions
void intakeForward(void);
void intakeStop(void);
void intakeBackward(void);

// output functions
void outputForward(void);
void outputStop(void);
void outputBackward(void);

// pneumatic toggle functions
void togglePistonA(void);
void togglePistonB(void);

// utility
int clamp(int value, int min, int max);
