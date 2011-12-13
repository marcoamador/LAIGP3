//
//  Peca.h
//  Laig3
//
//  Created by Pedro Borges on 12/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <vector>
#ifdef __APPLE__
#include <GLUI/GLUI.h>
#else
#include <gl/glui.h>
#endif

#ifndef Laig3_Peca_h
#define Laig3_Peca_h
extern int peca;
class Peca {
    static int id_s;
    int id;
public:
    Peca();
    int draw(GLenum mode);
};


#endif
