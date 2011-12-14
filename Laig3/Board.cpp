//
//  Board.cpp
//  Laig3
//
//  Created by Pedro Borges on 12/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Board.h"

static int size_casa=3;


int Board::moveto(int x, int y){
    float dx;
    float dy;
    if((float)x>(float)this->n_vertices/2.0){
        dx=(float)x-(float)this->n_vertices/2.0;
    }else{
        dx=-((float)this->n_vertices/2.0-(float)x);
    }
    if((float)y>(float)this->n_vertices/2.0){
        dy=(float)y-(float)this->n_vertices/2.0;
    }else{
        dy=-((float)this->n_vertices/2.0-(float)y);
    }
    dx+=0.5;dy+=0.5;
    glTranslated(dx*size_casa, 1, dy*size_casa);
}

int Board::processmove(int index,float x, float y, float z){
    int dx=0;
    int dz=0;
    if((x>=1.3) || (x<=-1.3) ){
        if(x>0){
            dx=1;
        }else{
            dx=-1;
        }
    }
    if((z>=1.3) || (z<=-1.3) ){
        if(z>0){
            dz=1;
        }else{
            dz=-1;
        }
    }
    
    if(dx!=0 || dz!=0)
    for (int i=0; i<this->board.size(); i++) {
        for (int j=0; j<this->board[i].size(); j++) {
            if(this->board[i][j]!=NULL){
                if (this->board[i][j]->getId()==index) {
                    if((j+dx)<this->board[i].size() && (i+dz)<this->board.size()){
                        if(this->board[i+dz][j+dx]==NULL){
                            this->board[i+dz][j+dx]=this->board[i][j];
                            this->board[i][j]=NULL;
                            i=this->board.size();
                            j=this->board[i].size();
                        }
                    }
                }
            }
        }
    }

    
    return 0;
}

Board::Board(unsigned int l){
    pix.makeCheckBoard2();				// cria texturas
	pix.setTexture(200);
    this->board.erase(board.begin(), board.end());
    vector<Peca *> b(l+1,NULL);
    vector<vector<Peca * > > a(l+1,b);   
    this->board=a;
    glNewList(5, GL_COMPILE);
	
    glPushMatrix();
    paralelo2(size_casa*l, 1, size_casa*l,200,l);
    glPopMatrix();
  
	glEndList();
    this->n_vertices=l+1;
    for(int x=1;x<n_vertices;x+=2){
        for(int y=1;y<4;y++){
            this->board[y][x]=new Peca(1);
        }
    }
    for(int x=0;x<n_vertices;x+=2){
        for(int y=6;y<n_vertices-1;y++){
            this->board[y][x]=new Peca(2);
        }
    }
    
}

int Board::draw(GLenum mode){
    if (mode == GL_SELECT)
		glLoadName (0);	
    glCallList(5);
    glPushMatrix();
    for (int i=0; i<this->board.size(); i++) {
        for (int j=0; j<this->board[i].size(); j++) {
            if(this->board[i][j]!=NULL){
                glPushMatrix();
                moveto(j, i);
                this->board[i][j]->draw(mode);
                glPopMatrix();
            }
        }
    }
    glPopMatrix();
    return 0;

};