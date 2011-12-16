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
#include "Peca.h"
#include "misc.h"
#include "RGBpixmap.h"
#ifndef Laig3_Board_h
#define Laig3_Board_h

using namespace std;

class Board {
    vector<vector<Peca*> > board;
    RGBpixmap pix;
    int n_vertices;
    int moveto(int x, int y);
    int select;
    int sx,sy;
public:
    Board();
    Board(unsigned int l);
    int draw(GLenum mode);
    int processmove(int index,float x, float y, float z);
    int tryselect(int i);
    int settabuleiro(vector<string> tab);
    string stinguify();
};

#endif
