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
    player=1;
}

int Peca::draw(GLenum mode){
    glEnable(GL_COLOR_MATERIAL);
    if (mode == GL_SELECT){
        std::cout<<"loaded id: "<<id<<std::endl;
		glLoadName (this->id);
    }	
    if(player==2){
        glColor3d(1.0, 0, 0);
    }else{
        glColor3d(0.0, 0.0, 1.0);
    }
    glCallList(peca);
    glColor3d(1.0, 1.0, 1.0);
    glDisable(GL_COLOR_MATERIAL);
    return 0;
}