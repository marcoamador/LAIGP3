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
    hidden=false;
     city=false;
    this->id=id_s;
    this->id_s++;
    player=1;
}



int Peca::draw(GLenum mode){
    if(!this->hidden){
    glEnable(GL_COLOR_MATERIAL);
    //glEnable (GL_BLEND);
    //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (mode == GL_SELECT){
      //  std::cout<<"loaded id: "<<id<<std::endl;
		glLoadName (this->id);
    }	
    if(player==2){
        glColor4d(1.0, 0, 0,0.6);
    }else{
        glColor4d(0.0, 0.0, 1.0,0.6);
    }
    if(!city2)
        glCallList(peca);
    else
        glCallList(city);
    glColor3d(1.0, 1.0, 1.0);
    glDisable(GL_COLOR_MATERIAL);
   //
    
        glDisable(GL_BLEND);}
    return 0;
}