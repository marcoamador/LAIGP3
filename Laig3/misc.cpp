//
//  misc.cpp
//  Laig3
//
//  Created by Pedro Borges on 13/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "misc.h"

void normalise(GLdouble *vec)
{
    GLdouble length = 0.0;
    int i;
    
    for (i=0;i<VLENGTH;i++){
        length += vec[i]*vec[i]; 
    }
    length= (GLdouble) sqrt((double)length); 
    
    for (i=0;i<VLENGTH;i++){
        vec[i] = vec[i]/length; 
    }
}


void newellSquare(GLdouble *vec1,GLdouble *vec2,GLdouble *vec3,GLdouble *vec4,GLdouble *normal)
{
    normal[0] = (vec1[1]-vec2[1])*(vec1[2]+vec2[2]) + 
    (vec2[1]-vec3[1])*(vec2[2]+vec3[2]) + 
    (vec3[1]-vec4[1])*(vec3[2]+vec4[2]) +
    (vec4[1]-vec1[1])*(vec4[2]+vec1[2]);
    normal[1] = (vec1[2]-vec2[2])*(vec1[0]+vec2[0]) + 
    (vec2[2]-vec3[2])*(vec2[0]+vec3[0]) + 
    (vec3[2]-vec4[2])*(vec3[0]+vec4[0]) +
    (vec4[2]-vec1[2])*(vec4[0]+vec1[0]);
    normal[2] = (vec1[0]-vec2[0])*(vec1[1]+vec2[1]) + 
    (vec2[0]-vec3[0])*(vec2[1]+vec3[1]) + 
    (vec3[0]-vec4[0])*(vec3[1]+vec4[1]) +
    (vec4[0]-vec1[0])*(vec4[1]+vec1[1]);
    
    normalise(normal);
    
}

void paralelo(GLdouble dimx, GLdouble dimy, GLdouble dimz){
	GLdouble dx=dimx/2, dy=dimy/2, dz=dimz/2;
	
	GLdouble v1[3] = {dx,-dy,dz};
	GLdouble v2[3] = {dx,-dy,-dz};
	GLdouble v3[3] = {dx,dy,dz};
	GLdouble v4[3] = {dx,dy,-dz};
	GLdouble v5[3] = {-dx,-dy,dz};
	GLdouble v6[3] = {-dx,dy,dz};
	GLdouble v7[3] = {-dx,dy,-dz};
	GLdouble v8[3] = {-dx,-dy,-dz};
	GLdouble normal[VLENGTH];
    
	float mat_shininess[] = {20.0}; /* How shiny is the object (specular exponent)  */
	float mat_specular[] = {1.0, 1.0, 1.0, 1.0}; /* specular reflection. */
	float mat_diffuse[] = {1.0, 1.00, 1.00, 1.0}; /* diffuse reflection. */
	// define as caracteristicas do material (dos materiais seguintes, i.e. ate nova alteracao
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    
    
	//Face frente - 0
	newellSquare(v1,v3,v6,v5,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glVertex3dv(v1);
    glVertex3dv(v3);
    glVertex3dv(v6);
    glVertex3dv(v5);
	glEnd();
    
	// face anterior - 1
	newellSquare(v8,v7,v4,v2,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glVertex3dv(v8);
    glVertex3dv(v7);
    glVertex3dv(v4);
    glVertex3dv(v2);
	glEnd();
    
	// face lateral - 2
	newellSquare(v2,v4,v3,v1,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glVertex3dv(v2);
    glVertex3dv(v4);
    glVertex3dv(v3);
    glVertex3dv(v1);
	glEnd();
    
	newellSquare(v5,v6,v7,v8,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glVertex3dv(v5);
    glVertex3dv(v6);
    glVertex3dv(v7);
    glVertex3dv(v8);
	glEnd();
    
	// base
	newellSquare(v1,v5,v8,v2,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glVertex3dv(v1);
    glVertex3dv(v5);
    glVertex3dv(v8);
    glVertex3dv(v2);
	glEnd();
    
    
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 2001);			// activa a textura 2001
    
	// topo 
	newellSquare(v3,v4,v7,v6,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glTexCoord2f(0.0,0.0); glVertex3dv(v3);
    glTexCoord2f(1.0,0.0); glVertex3dv(v4);
    glTexCoord2f(1.0,1.0); glVertex3dv(v7);
    glTexCoord2f(0.0,1.0); glVertex3dv(v6);
	glEnd();
	glDisable(GL_TEXTURE_2D);
    
}

void paralelo2(GLdouble dimx, GLdouble dimy, GLdouble dimz,int tex,int l){
	GLdouble dx=dimx/2, dy=dimy/2, dz=dimz/2;
	
	GLdouble v1[3] = {dx,-dy,dz};
	GLdouble v2[3] = {dx,-dy,-dz};
	GLdouble v3[3] = {dx,dy,dz};
	GLdouble v4[3] = {dx,dy,-dz};
	GLdouble v5[3] = {-dx,-dy,dz};
	GLdouble v6[3] = {-dx,dy,dz};
	GLdouble v7[3] = {-dx,dy,-dz};
	GLdouble v8[3] = {-dx,-dy,-dz};
	GLdouble normal[VLENGTH];
    
	float mat_shininess[] = {20.0}; /* How shiny is the object (specular exponent)  */
	float mat_specular[] = {1.0, 1.0, 1.0, 1.0}; /* specular reflection. */
	float mat_diffuse[] = {1.0, 1.00, 1.00, 1.0}; /* diffuse reflection. */
	// define as caracteristicas do material (dos materiais seguintes, i.e. ate nova alteracao
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    
    
	//Face frente - 0
	newellSquare(v1,v3,v6,v5,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glVertex3dv(v1);
    glVertex3dv(v3);
    glVertex3dv(v6);
    glVertex3dv(v5);
	glEnd();
    
	// face anterior - 1
	newellSquare(v8,v7,v4,v2,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glVertex3dv(v8);
    glVertex3dv(v7);
    glVertex3dv(v4);
    glVertex3dv(v2);
	glEnd();
    
	// face lateral - 2
	newellSquare(v2,v4,v3,v1,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glVertex3dv(v2);
    glVertex3dv(v4);
    glVertex3dv(v3);
    glVertex3dv(v1);
	glEnd();
    
	newellSquare(v5,v6,v7,v8,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glVertex3dv(v5);
    glVertex3dv(v6);
    glVertex3dv(v7);
    glVertex3dv(v8);
	glEnd();
    
	// base
	newellSquare(v1,v5,v8,v2,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glVertex3dv(v1);
    glVertex3dv(v5);
    glVertex3dv(v8);
    glVertex3dv(v2);
	glEnd();
    
    
	
    double n=(double)l/4.0;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);			// activa a textura 2001
    
	// topo 
	newellSquare(v3,v4,v7,v6,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glTexCoord2f(0.0,0.0); glVertex3dv(v3);
    glTexCoord2f(n,0.0); glVertex3dv(v4);
    glTexCoord2f(n,n); glVertex3dv(v7);
    glTexCoord2f(0.0,n); glVertex3dv(v6);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
    
}

void paralelo3(GLdouble dimx, GLdouble dimy, GLdouble dimz,int tex,int l){
	GLdouble dx=dimx/2, dy=dimy/2, dz=dimz/2;
	
	GLdouble v1[3] = {dx,-dy,dz};
	GLdouble v2[3] = {dx,-dy,-dz};
	GLdouble v3[3] = {dx,dy,dz};
	GLdouble v4[3] = {dx,dy,-dz};
	GLdouble v5[3] = {-dx,-dy,dz};
	GLdouble v6[3] = {-dx,dy,dz};
	GLdouble v7[3] = {-dx,dy,-dz};
	GLdouble v8[3] = {-dx,-dy,-dz};
	GLdouble normal[VLENGTH];
    
	float mat_shininess[] = {20.0}; /* How shiny is the object (specular exponent)  */
	float mat_specular[] = {1.0, 1.0, 1.0, 1.0}; /* specular reflection. */
	float mat_diffuse[] = {1.0, 1.00, 1.00, 1.0}; /* diffuse reflection. */
	// define as caracteristicas do material (dos materiais seguintes, i.e. ate nova alteracao
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    
	double n=(double)l;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
    
	//Face frente - 0
	newellSquare(v1,v3,v6,v5,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glTexCoord2f(n,0.0); glVertex3dv(v1);
    glTexCoord2f(n,n); glVertex3dv(v3);
    glTexCoord2f(0.0,n); glVertex3dv(v6);
    glTexCoord2f(0.0,0.0); glVertex3dv(v5);
	glEnd();

	glDisable(GL_TEXTURE_2D);
    
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	// face anterior - 1
	newellSquare(v8,v7,v4,v2,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glTexCoord2f(0.0,0.0);glVertex3dv(v8);
    glTexCoord2f(0.0, n); glVertex3dv(v7);
    glTexCoord2f(n,n); glVertex3dv(v4);
    glTexCoord2f(n, 0.0); glVertex3dv(v2);
	glEnd();

	glDisable(GL_TEXTURE_2D);
    
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	// face lateral - 2
	newellSquare(v2,v4,v3,v1,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glTexCoord2f(n,0);glVertex3dv(v2);
     glTexCoord2f(n,n);glVertex3dv(v4);
    glTexCoord2f(0,n); glVertex3dv(v3);
    glTexCoord2f(0.0,0.0);glVertex3dv(v1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
    
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	newellSquare(v5,v6,v7,v8,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
     glTexCoord2f(n,0.0);glVertex3dv(v5);
     glTexCoord2f(n,n);glVertex3dv(v6);
    glTexCoord2f(0,n);glVertex3dv(v7);
    glTexCoord2f(0.0,0.0);glVertex3dv(v8);
	glEnd();

	glDisable(GL_TEXTURE_2D);
    
	// base
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 2002);
	newellSquare(v1,v5,v8,v2,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
     glTexCoord2f(0.0,0.0);glVertex3dv(v1);
    glTexCoord2f(3,0.0); glVertex3dv(v5);
    glTexCoord2f(3,3); glVertex3dv(v8);
    glTexCoord2f(0.0,3);glVertex3dv(v2);
	glEnd();

	glDisable(GL_TEXTURE_2D);
    
    
	
    
	// topo 
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 2004);
	newellSquare(v3,v4,v7,v6,normal);
	glBegin(GL_POLYGON);
    glNormal3dv(normal);
    glTexCoord2f(0.0,0.0); glVertex3dv(v3);
    glTexCoord2f(1,0.0); glVertex3dv(v4);
    glTexCoord2f(1,1); glVertex3dv(v7);
    glTexCoord2f(0.0,1); glVertex3dv(v6);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
    
}
