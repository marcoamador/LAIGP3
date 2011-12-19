//
//  misc.h
//  Laig3
//
//  Created by Pedro Borges on 13/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#pragma once
#include <math.h>
#ifdef __APPLE__
#include <GLUI/GLUI.h>
#else
#include <gl/glui.h>
#endif
#ifndef Laig3_misc_h
#define Laig3_misc_h
#define VLENGTH 3
/* Function to normalise a vector to unit length */

void normalise(GLdouble *vec);


void newellSquare(GLdouble *vec1,GLdouble *vec2,GLdouble *vec3,GLdouble *vec4,GLdouble *normal);

void paralelo(GLdouble dimx, GLdouble dimy, GLdouble dimz);

void paralelo2(GLdouble dimx, GLdouble dimy, GLdouble dimz,int texture,int l);
void paralelo3(GLdouble dimx, GLdouble dimy, GLdouble dimz,int tex,int l);
void paralelo4(GLdouble dimx, GLdouble dimy, GLdouble dimz,int tex,int l);

#endif
