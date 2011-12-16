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
    if((x>=1.0) || (x<=-1.0) ){
        if(x>0){
            dx=1;
        }else{
            dx=-1;
        }
    }
    if((z>=1.0) || (z<=-1.0) ){
        if(z>0){
            dz=1;
        }else{
            dz=-1;
        }
    }
    cout<<"Peca id: "<<index<<" dx: "<<" dz: "<<dz<<endl;
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
                            cout<<"Peca id: "<<index<<" moved!"<<endl;
                            return 1;
                        }
                    }
                }
            }
        }
    }

    
    return 0;
}


int Board::settabuleiro(vector<string> tab){
    for(int i=0; i<this->board.size();i++){
        for(int j=0;j<this->board[i].size();j++){
            if(this->board[i][j]!=NULL){
                delete this->board[i][j];
                this->board[i][j]=NULL;
            }
        }
    }
    Peca::id_s=1;
    for(int i=0; i<tab.size();i++){
        for(int j=0;j<tab[i].size();j++){
            if(tab[i][j]!='x'){
                if(tab[i][j]=='r'){
                this->board[i][j]=new Peca(1);
                }else{
                this->board[i][j]=new Peca(2);
                }
            }
        }
    }
    return 0;
}

string Board::stinguify(){
    string a="[";
    for(int i=0; i<this->board.size();i++){
        a+='[';
        for(int j=0;j<this->board[i].size();j++){
            if(this->board[i][j]==NULL){
                a+='x';
            }else{
                if(this->board[i][j]->getPlayer()==1){
                    a+='r';
                }else{
                    a+='b';
                }
            }
            if(j<this->board[i].size()-1){
                a+=',';
            }else{
                a+=']';
            }
        }
        if(i<this->board.size()-1){
            a+=',';
        }else{
            a+=']';
        }

    }
    return a;
}

Board::Board(unsigned int l){
    select=-1;
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
    this->n_vertices=l+1;/*
    for(int x=1;x<n_vertices;x+=2){
        for(int y=1;y<4;y++){
            this->board[y][x]=new Peca(1);
        }
    }
    for(int x=0;x<n_vertices;x+=2){
        for(int y=6;y<n_vertices-1;y++){
            this->board[y][x]=new Peca(2);
        }
    }*/
    
}

int Board::tryselect(int index){
    cout<<"Stringified: "<<this->stinguify()<<endl;
    if(select<0){ 
        cout<<"selecionada: "<<index<<endl;
        for (int i=0; i<this->board.size(); i++) {
        for (int j=0; j<this->board[i].size(); j++) {
            if(this->board[i][j]!=NULL){
                if(this->board[i][j]->getId()==index){
                    select=index;
                    sx=j;
                    sy=i;
                    return 1;
                }
            }
        }
       }
    }else{
        cout<<"casa selecionada: "<<index<<endl;
        int max=this->n_vertices*this->n_vertices+1;
        for (int i=0; i<this->board.size(); i++) {
            for (int j=0; j<this->board[i].size(); j++) {
                //cout<<"Max: "<<max<<endl;
                    if(max==index){
                        this->board[i][j]=this->board[sy][sx];
                        this->board[sy][sx]=NULL;
                        select=-1;
                        return 2;
                        
                    }
                
                max++;
            }
        }
    
    }
    return 0;
}

int Board::draw(GLenum mode){
    if (mode == GL_SELECT)
		glLoadName (0);	
    glCallList(5);
    glPushMatrix();
    int max=this->n_vertices*this->n_vertices+1;
    for (int i=0; i<this->board.size(); i++) {
        for (int j=0; j<this->board[i].size(); j++) {
           if(this->board[i][j]==NULL){ 
                glPushMatrix();
                 moveto(j, i);
            glPushName(max);
            glBegin(GL_POLYGON);
            glVertex3d(-0.5, -0.49, 0.5);
            glVertex3d(-0.5, -0.49, -0.5);
             glVertex3d(0.5, -0.49, -0.5);
             glVertex3d(0.5, -0.49, 0.5);
            glEnd();
                
                glPopMatrix();
               }
            max++;
            
        }
    }
    for (int i=0; i<this->board.size(); i++) {
        for (int j=0; j<this->board[i].size(); j++) {
            if(this->board[i][j]!=NULL){
                glPushMatrix();
                moveto(j, i);
                if(this->board[i][j]->getId()==select){
                    glEnable (GL_BLEND);
                    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glEnable(GL_COLOR_MATERIAL);
                    glColor4d(0, 1, 0,0.4);
                    glPushName(max);
                    glBegin(GL_POLYGON);
                    glVertex3d(-(float)size_casa/2.0, -0.49, (float)size_casa/2.0);
                    glVertex3d(-(float)size_casa/2.0, -0.49, -(float)size_casa/2.0);
                    glVertex3d((float)size_casa/2.0, -0.49, -(float)size_casa/2.0);
                    glVertex3d((float)size_casa/2.0, -0.49, (float)size_casa/2.0);
                    glEnd();  
                    glColor3d(1, 1, 1);
                    glDisable(GL_COLOR_MATERIAL);
                    glDisable(GL_BLEND);
                    
                }

                this->board[i][j]->draw(mode);
                glPopMatrix();
            }
        }
    }
    glPopMatrix();
    return 0;

};