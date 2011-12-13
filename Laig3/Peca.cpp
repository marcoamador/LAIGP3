//
//  Peca.cpp
//  Laig3
//
//  Created by Pedro Borges on 12/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Peca.h"

int Peca::id_s=1;

Peca::Peca(){
    this->id=id_s;
    this->id_s++;
}

int Peca::draw(GLenum mode){
    if (mode == GL_SELECT)
		glLoadName (this->id);	
    glCallList(peca);
    return 0;
}