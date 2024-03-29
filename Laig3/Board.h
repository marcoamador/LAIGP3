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

struct pospeca{
    int i;
    int j;
    int player;
    bool city;

};

struct jogada {
    vector<pospeca> entrada;
    vector<pospeca> saida;
};

using namespace std;

class Board {
    vector<struct jogada> jogadas;
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
    int frame;
    bool filme;
    bool frameready;
    vector<mov> movi;
    vector<mov> differences(vector<string> &newvec,bool);
    vector<mov> differences(vector<string> &newvec);
    pair<float,float> getxy(int x,int y);
    string stinguify(vector<vector<Peca*> >);
    int settabuleiro(vector<string> tab,bool diff);
    int playframe();
    vector<Peca*> drawerp1;
    vector<Peca*> drawerp2;
    bool player1;
    bool ai1;
    bool ai2;
    int winner;
public:
    Board();
    ~Board();
    Board(unsigned int l);
     Board(unsigned int l, Socket *s);
    int draw(GLenum mode);
    int processmove(int index,float x, float y, float z);
    int tryselect(int i,int jogador);
    int selectcity(int index, int player);
    int settabuleiro(vector<string> tab);
    int go_back();
    string stinguify();
    string strexecutamov(int player, int opt,int xi,int yi,int xf,int yf);
    string strverificafim();
    void ai_play(int player, int level);
    int play();
    void nextframe( int i);
    bool is_player1ai(){
        return ai1;
    }
    bool is_player2ai(){
        return ai2;
    }
    void setAi1(bool a){
        ai1=a;
    }
    void setAi2(bool a){
        ai2=a;
    }
    bool is_stopped(){
        return movi.size()==0;
    }
    bool is_playing(){
    
        return this->filme;
    }
    int exit_sock(){
        sock->closes();
        cout<<sock->sendandreceive("bye.\n")<<endl;
        
        return 0;
    }
    bool rotateview(){
        return !player1;
    }
    int getWinner(){
        return winner;
    }
    Socket * getsocket(){
        return sock;
    }
    void  setsocket(Socket *s){
        sock=s;
    }
    
};

#endif
