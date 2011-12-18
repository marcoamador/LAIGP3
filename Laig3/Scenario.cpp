#include "Scenario.h"
#include "misc.h"

Scenario::Scenario(void)
{
}

void Scenario::draw(){
	glPushMatrix();
		glTranslated(0,40,0);
		paralelo3(200.0, 200.0, 200.0, 2003, 1);
	glPopMatrix();
}