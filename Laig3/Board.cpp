//
//  Board.cpp
//  Laig3
//
//  Created by Pedro Borges on 12/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Board.h"

static int size_casa=1;

Board::Board(unsigned int l){
    pix.makeCheckBoard2();				// cria texturas
	pix.setTexture(200);
    this->board.erase(board.begin(), board.end());
    vector<Peca *> b(l+1,NULL);
    vector<vector<Peca * > > a(l+1,b);    
    glNewList(3, GL_COMPILE);
	
    glPushMatrix();
    paralelo2(size_casa*l, size_casa, size_casa*l,200,l);
    glPopMatrix();
  
	glEndList();
    
}

int Board::draw(){
    glCallList(3);
    return 0;

};