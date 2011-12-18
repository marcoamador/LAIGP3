#pragma once

#include <math.h>
#ifdef __APPLE__
#include <GLUI/GLUI.h>
#else
#include <gl/glui.h>
#include "misc.h"
#endif

#ifndef Laig3_scenario_h
#define Laig3_scenario_h

class Scenario
{
public:
	Scenario(void);
	void draw();
};

#endif

