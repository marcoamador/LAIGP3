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
	//Tex1 - cilindro
	//Tex2 - mesa
	//Tex3 - cenário circundante
	//Tex4 - solo
	int tex1, tex2, tex3, tex4;
	Scenario(void);
	void draw(int, int, int, int);
};

#endif

