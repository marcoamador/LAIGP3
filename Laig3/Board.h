//
//  Board.h
//  Laig3
//
//  Created by Pedro Borges on 12/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <vector>
#include "Peca.h"
#ifndef Laig3_Board_h
#define Laig3_Board_h

using namespace std;

class Board {
    vector<vector<Peca*> > board;
    
public:
    Board();
    Board(int c, int l);
    int draw();
};

#endif
