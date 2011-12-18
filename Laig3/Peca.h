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
extern int city;
class Peca {
        int id;
    int player;
    bool hidden;
    bool city2;
public:
    static int id_s;
    Peca();
    Peca(int player){
        hidden=false;
        city2=false;
        this->id=this->id_s;
        this->id_s++;
        this->player=player;
    };
    bool makecity(){
        city2=true;
        return true;
    }
    void set_hidden(bool hid){
        hidden=hid;
    }
    int draw(GLenum mode);
    int getId(){
        return id;
    }
    int getPlayer(){
        return player;
    
    }
    int is_city(){
        return city2;
    }
};


#endif
