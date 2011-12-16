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
        int id;
    int player;
public:
    static int id_s;
    Peca();
    Peca(int player){
        this->id=this->id_s;
        this->id_s++;
        this->player=player;
    };
    int draw(GLenum mode);
    int getId(){
        return id;
    }
    int getPlayer(){
        return player;
    
    }
};


#endif
