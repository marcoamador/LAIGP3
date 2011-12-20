#include "Scenario.h"
#include "misc.h"

Scenario::Scenario(void)
{
}

void Scenario::draw(int tex1, int tex2, int tex3, int tex4){
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
    glBindTexture(GL_TEXTURE_2D,tex1);
    gluCylinder(glQ,5.5,5.5,23.5,10,10);
    glPopMatrix();
    
    
    
    paralelo4(40, 3, 40, tex2, 1);
    glPopMatrix();
    
    glTranslated(0,100,0);
    paralelo3(200.0, 250.0, 200.0, tex3, 1, tex4);
	glPopMatrix();
}