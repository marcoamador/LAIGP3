#include "Scenario.h"
#include "misc.h"

Scenario::Scenario(void)
{
}

void Scenario::draw(){
	glPushMatrix();
    glPushMatrix();
    glTranslated(0,-1.5 ,0);
    glPushMatrix();  
    GLUquadric* glQ = gluNewQuadric();
    gluQuadricDrawStyle(glQ, GLU_FILL);             
    gluQuadricNormals(glQ, GLU_SMOOTH);             
    gluQuadricOrientation(glQ, GLU_OUTSIDE);
    gluQuadricTexture(glQ, GL_TRUE);
    glRotated(90,1,0,0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2006);
    gluCylinder(glQ,5.5,5.5,23.5,10,10);
    glPopMatrix();
    
    
    
    paralelo4(40, 3, 40, 2005, 1);
    glPopMatrix();
    
    glTranslated(0,100,0);
    paralelo3(200.0, 250.0, 200.0, 2003, 1);
	glPopMatrix();
}