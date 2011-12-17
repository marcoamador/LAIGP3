//
//  Board.h
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

#include <sstream>
#include "Peca.h"
#include "misc.h"
#include "RGBpixmap.h"
#include "sockets.h"
#ifndef Laig3_Board_h
#define Laig3_Board_h


struct mov {
    int inii;
    int inij;
    int fini;
    int finj;
    int peca;
    pair<float,float> inipos;
    pair<float,float> finpos;
    Peca *ptr;
    float altura;
};

using namespace std;

class Board {
    vector<vector<Peca*> > board;
    RGBpixmap pix;
    int n_vertices;
    int moveto(int x, int y);
    int select;
    int sx,sy;
    Socket * sock;
    bool city;
    int contagem;
    bool fim;
    vector<mov> movi;
    vector<mov> differences(vector<string> &newvec);
    pair<float,float> getxy(int x,int y);
public:
    Board();
    Board(unsigned int l);
    int draw(GLenum mode);
    int processmove(int index,float x, float y, float z);
    int tryselect(int i,int jogador);
    int selectcity(int index, int player);
    int settabuleiro(vector<string> tab);
    string stinguify();
    string strexecutamov(int player, int opt,int xi,int yi,int xf,int yf);
    string strverificafim();
};

#endif
