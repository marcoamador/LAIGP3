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

pair<float,float> Board::getxy(int x,int y){
    pair<float,float> a;
    if(x<0 || y<0){
        float dx;
        float dy;
        if(x==-1)
        {
            dx=(float)this->n_vertices/2.0*size_casa;
            dy=((float)-this->n_vertices/2.0+abs(y))*size_casa;
        }
        else{
            dx=(float)-this->n_vertices/2.0*size_casa;
            dy=((float)this->n_vertices/2.0-abs(y))*size_casa;
        }

        //glTranslated(dx, 1, dy);
        a.first=dx;
        a.second=dy;
    }else{
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
    a.first=dx*size_casa;
        a.second=dy*size_casa;
    }
    //glTranslated(dx*size_casa, 1, dy*size_casa);
    return a;
}


int Board::moveto(int x, int y){
    if(x<0 || y<0){
        float dx;
        float dy;
        if(x==-1)
        {
            dx=(float)this->n_vertices/2.0*size_casa;
            dy=((float)-this->n_vertices/2.0+abs(y))*size_casa;
        }
        else{
            dx=(float)-this->n_vertices/2.0*size_casa;
            dy=((float)this->n_vertices/2.0-abs(y))*size_casa;
        }
        glTranslated(dx, 1, dy);
    
    }else{
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
    return 0;
}

int Board::play(){
    this->frame=0;
    this->filme=true;
    for (int i=0; i<drawerp1.size(); i++) {
        delete drawerp1[i];
    }
    for (int i=0; i<drawerp2.size(); i++) {
        delete drawerp2[i];
    }
    drawerp1.erase(drawerp1.begin(), drawerp1.end());
    drawerp2.erase(drawerp2.begin(), drawerp2.end());
    Peca * p=new Peca(1);
    p->makecity();
    drawerp1.push_back(p);
    /*p=new Peca(1);
     
     drawerp1.push_back(p);*/
    p=new Peca(2);
    p->makecity();
    
    drawerp2.push_back(p);
    this->settabuleiro(sock->slitarray(sock->innerfunc(sock->sendandreceive("initialize.\n"))),false);
    
    this->frameready=true;
    return 0;
}

int Board::playframe(){
    if(this->frame<this->jogadas.size() ){
        this->frameready=false;
        vector<vector<Peca*> > tab=this->board;
        struct jogada j=this->jogadas[this->frame];
        //this->jogadas.erase(this->jogadas.end()-1);
        for (int i=0; i<j.entrada.size(); i++) {
            /*if(tab[j.entrada[i].i][j.entrada[i].j]!=NULL){
             delete tab[j.entrada[i].i][j.entrada[i].j];
             }*/
            Peca * p=new Peca(j.entrada[i].player);
            if(j.entrada[i].city)
                p->makecity();
            tab[j.entrada[i].i][j.entrada[i].j]=p;
            
        }
        for (int i=0; i<j.saida.size(); i++) {
            /* if(this->board[j.saida[i].i][j.saida[i].j]!=NULL){
             delete this->board[j.saida[i].i][j.saida[i].j];
             }*/
            tab[j.saida[i].i][j.saida[i].j]=NULL;
        }
        this->settabuleiro(sock->slitarray(this->stinguify(tab)), false);
    }
    return 0;
}


void Board::nextframe(int i){
    if(filme){
        if(this->frameready && this->frame<this->jogadas.size()){
            
            this->playframe();
            this->frame++;
        }else{
            if(this->frameready){
            this->filme=false;
                this->frame=1;}
            
        }
    }
    return;
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
    return this->settabuleiro( tab,true);
}

int Board::go_back(){
    if(jogadas.size()>1){
        
        select=-1;
    vector<vector<Peca*> > tab=this->board;
    struct jogada j=*(this->jogadas.end()-1);
    this->jogadas.erase(this->jogadas.end()-1);
    for (int i=0; i<j.entrada.size(); i++) {
        /*if(tab[j.entrada[i].i][j.entrada[i].j]!=NULL){
            delete tab[j.entrada[i].i][j.entrada[i].j];
        }*/
        tab[j.entrada[i].i][j.entrada[i].j]=NULL;
    }
    for (int i=0; i<j.saida.size(); i++) {
       /* if(this->board[j.saida[i].i][j.saida[i].j]!=NULL){
            delete this->board[j.saida[i].i][j.saida[i].j];
        }*/
        Peca * p=new Peca(j.saida[i].player);
        if(j.saida[i].city)
            p->makecity();
        tab[j.saida[i].i][j.saida[i].j]=p;
    }
        this->settabuleiro(sock->slitarray(this->stinguify(tab)), false);
    player1=!player1;
    }
    
    return 0;
}

int Board::settabuleiro(vector<string> tab,bool diff){

 
        this->movi=differences(tab,diff);
   
    for(int i=0; i<this->board.size();i++){
        for(int j=0;j<this->board[i].size();j++){
            if(this->board[i][j]!=NULL){
                delete this->board[i][j];
                this->board[i][j]=NULL;
            }
        }
    }
    Peca::id_s=1;
    //vector<vector<Peca *> > tabt=board;
    for(int i=0; i<tab.size();i++){
        for(int j=0;j<tab[i].size();j++){
            if(tab[i][j]!='x'){
                if(tab[i][j]=='r'){
                board[i][j]=new Peca(1);
                }else{
                    if(tab[i][j]=='b')
                        board[i][j]=new Peca(2);
                    else
                        if (tab[i][j]=='t') {
                            board[i][j]=new Peca(1);
                            board[i][j]->makecity();
                        }else{
                            board[i][j]=new Peca(2);
                            board[i][j]->makecity();
                        }
                }
            }
        }
    }
    string winner=sock->innerfunc(sock->sendandreceive(this->strverificafim()));
    cout<<"winner: "<<winner<<endl;
    if(winner!="-1"){
        return atoi(winner.c_str());
    }
    for (int i=0; i<movi.size();i++) {
        if(movi[i].finj>=0)
        board[movi[i].fini][movi[i].finj]->set_hidden(true);
    }
   
    
    return 0;
}
//execute(Mov, Board), ok(NewBoard)):-Mov=[Jogador,Opcao,L_ini,C_ini,L_fin,C_fin]

vector<mov> Board::differences(vector<string> &newvec){
    return differences(newvec,true);
}




vector<mov> Board::differences(vector<string> &newvec,bool newjog){
   // vector<string> newvec=sock->slitarray(newBoard);
    struct  jogada jog;
    vector<pair<int, int> > entradas;
    vector<pair<int, int> > saidas;
    vector<mov> movement;
    for(int i=0; i<newvec.size();i++){
        for(int j=0;j<newvec[i].size();j++){
            pair<int, int> a;
            if(this->board[i][j]==NULL && newvec[i][j]!='x'){
                a.first=i;
                a.second=j;
                struct pospeca b;
                b.i=i;
                b.j=j;
                b.player=Peca::char2player(newvec[i][j]);
                b.city=Peca::char2city(newvec[i][j]);
                jog.entrada.push_back(b);
                cout<<"entrada char: "<<newvec[i][j]<<" i: "<<i<<" j:"<<j<<" cidade: "<<b.city<<endl;
                entradas.push_back(a);
            }else{
                if(this->board[i][j]!=NULL && newvec[i][j]=='x'){
                    a.first=i;
                    a.second=j;
                    cout<<"saida "<<i<<" j:"<<j<<endl;
                    saidas.push_back(a);
                    struct pospeca b;
                    b.i=i;
                    b.j=j;
                    b.player=this->board[i][j]->getPlayer();
                    b.city=this->board[i][j]->is_city();
                    cout<<" is_city "<<b.city<<endl;
                    jog.saida.push_back(b);

                } 
            }
        }
    }
    if(this->frame==0 &&filme==true)
        return movement;
    if(entradas.size()>saidas.size()){
        for(int i=0; i<entradas.size();i++){
            if(Peca::char2player(newvec[entradas[i].first][entradas[i].second])==1)
                for(int j=0;j<drawerp1.size();j++){
                    if(drawerp1[j]->is_city()==Peca::char2city(newvec[entradas[i].first][entradas[i].second])){
                        struct mov tmp;
                        tmp.inii=j;
                        tmp.inij=-1;
                        tmp.fini=entradas[i].first;
                        tmp.finj=entradas[i].second;
                        tmp.peca=Peca::char2player(newvec[entradas[i].first][entradas[i].second]);
                        tmp.inipos=getxy(tmp.inij, tmp.inii);
                        cout<<"ini: x: "<<tmp.inipos.first<<" y: "<<tmp.inipos.second<<endl;
                        tmp.finpos=getxy(tmp.finj, tmp.fini);
                        cout<<"fin: x: "<<tmp.finpos.first<<" y: "<<tmp.finpos.second<<endl;
                        tmp.ptr=new Peca(tmp.peca);
                        if(drawerp1[j]->is_city())
                            tmp.ptr->makecity();
                        tmp.altura=1;
                        movement.push_back(tmp);
                        drawerp1.erase(drawerp1.begin()+j);
                        break;

                    }
                }
            
        else{
        
            if(Peca::char2player(newvec[entradas[i].first][entradas[i].second])==2)
                for(int j=0;j<drawerp2.size();j++){
                    if(drawerp2[j]->is_city()==Peca::char2city(newvec[entradas[i].first][entradas[i].second])){
                        struct mov tmp;
                        tmp.inii=j;
                        tmp.inij=-2;
                        tmp.fini=entradas[i].first;
                        tmp.finj=entradas[i].second;
                        tmp.peca=Peca::char2player(newvec[entradas[i].first][entradas[i].second]);
                        tmp.inipos=getxy(tmp.inij, tmp.inii);
                        cout<<"ini: x: "<<tmp.inipos.first<<" y: "<<tmp.inipos.second<<endl;
                        tmp.finpos=getxy(tmp.finj, tmp.fini);
                        cout<<"fin: x: "<<tmp.finpos.first<<" y: "<<tmp.finpos.second<<endl;
                        tmp.ptr=new Peca(tmp.peca);
                        if(drawerp2[j]->is_city())
                            tmp.ptr->makecity();
                        tmp.altura=1;
                        movement.push_back(tmp);
                        drawerp2.erase(drawerp2.begin()+j);
                        break;
                        
                    }
                }

        }
        
        }
    
    }
    if(saidas.size()>entradas.size()){
        for (int i=0; i<saidas.size(); i++) {
            struct mov tmp;
            tmp.inii=saidas[i].first;
            tmp.inij=saidas[i].second;
            if(newjog){
            if(this->board[tmp.inii][tmp.inij]->getPlayer()==2){
                tmp.finj=-1;
                tmp.fini=drawerp1.size();
            }else{
                tmp.finj=-2;
                tmp.fini=drawerp2.size();;
            }
            }else{
                if(this->board[tmp.inii][tmp.inij]->getPlayer()==1){
                    tmp.finj=-1;
                    tmp.fini=drawerp1.size();
                }else{
                    tmp.finj=-2;
                    tmp.fini=drawerp2.size();;
                }

            }
            
            tmp.peca=this->board[tmp.inii][tmp.inij]->getPlayer();

            tmp.inipos=getxy(tmp.inij, tmp.inii);
            cout<<"ini: x: "<<tmp.inipos.first<<" y: "<<tmp.inipos.second<<endl;
            tmp.finpos=getxy(tmp.finj, tmp.fini);
            cout<<"fin: x: "<<tmp.finpos.first<<" y: "<<tmp.finpos.second<<endl;
            tmp.ptr=new Peca(tmp.peca);
            if(this->board[tmp.inii][tmp.inij]->is_city())
                tmp.ptr->makecity();
            tmp.altura=1;
            movement.push_back(tmp);
        }
    
    
    }
    if(entradas.size()==saidas.size()){
        for(int i=0; i<entradas.size();i++){
            struct mov tmp;
            tmp.inii=saidas[i].first;
            tmp.inij=saidas[i].second;
            tmp.fini=entradas[i].first;
            tmp.finj=entradas[i].second;
            tmp.peca=this->board[tmp.inii][tmp.inij]->getPlayer();
            tmp.inipos=getxy(tmp.inij, tmp.inii);
            cout<<"ini: x: "<<tmp.inipos.first<<" y: "<<tmp.inipos.second<<endl;
            tmp.finpos=getxy(tmp.finj, tmp.fini);
            cout<<"fin: x: "<<tmp.finpos.first<<" y: "<<tmp.finpos.second<<endl;
            tmp.ptr=new Peca(tmp.peca);
            tmp.altura=1;
            movement.push_back(tmp);
        }
    }
    if(newjog)
    this->jogadas.push_back(jog);
    return movement;
}

string Board::stinguify(vector<vector<Peca*> > board){
    string a="[";
    for(int i=0; i<board.size();i++){
        a+='[';
        for(int j=0;j<board[i].size();j++){
            if(board[i][j]==NULL){
                a+='x';
            }else{
                if(board[i][j]->is_city()){
                    if(board[i][j]->getPlayer()==1){
                        a+='t';
                    }else{
                        a+='y';
                    }
                    
                }else
                    if(board[i][j]->getPlayer()==1){
                        a+='r';
                    }else{
                        a+='b';
                    }
            }
            if(j<board[i].size()-1){
                a+=',';
            }else{
                a+=']';
            }
        }
        if(i<board.size()-1){
            a+=',';
        }else{
            a+=']';
        }
        
    }
    return a;


}


string vector2string(vector<int> num){
    //string a="[";
    stringstream a;
    a<<"[";
    for (int i=0; i<num.size(); i++) {
        if(num[i]>=0){
            //a+=('0'+num[i]);
            a<<num[i];
            if((i+1)<num.size()){
                if(num[i+1]<0){
                    //a+=',';
                }else{
                    a<<',';
                }

            }else{
                if(i<num.size()-1){
                    a<<',';
                }else{
                    a<<']';
                }
            }
        }else{
            if(i<num.size()-1){
               // a+=',';
            }else{
                a<<']';
            }

        }
    }
    return a.str();
}

string Board::strverificafim(){ 
        return "game_end("+this->stinguify()+").\n";
    
}

string Board::strexecutamov(int player, int opt,int xi,int yi,int xf,int yf){ 
    int arr[]={player,opt,xi,yi,xf,yf};
    vector<int> a(arr,arr+sizeof(arr) / sizeof(int));
    return "execute("+vector2string(a)+","+this->stinguify()+").\n";
    
}

string Board::stinguify(){
    string a="[";
    for(int i=0; i<this->board.size();i++){
        a+='[';
        for(int j=0;j<this->board[i].size();j++){
            if(this->board[i][j]==NULL){
                a+='x';
            }else{
                if(this->board[i][j]->is_city()){
                    if(this->board[i][j]->getPlayer()==1){
                        a+='t';
                    }else{
                        a+='y';
                    }

                }else
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
    city=false;    
    filme=false;
    frameready=false;
    player1=true;
    contagem=0;
    select=-1;
    this->sock=new Socket("127.0.0.1",60001);
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
    this->settabuleiro(sock->slitarray(sock->innerfunc(sock->sendandreceive("initialize.\n"))));
    /*
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
    Peca * p=new Peca(1);
    p->makecity();
    drawerp1.push_back(p);
    /*p=new Peca(1);
   
    drawerp1.push_back(p);*/
    p=new Peca(2);
    p->makecity();
    
    drawerp2.push_back(p);
    /*p=new Peca(2);
    drawerp2.push_back(p);
    p=new Peca(2);
    drawerp2.push_back(p);*/
}

int Board::tryselect(int index, int jogador1){
    if(filme){
        return 0;
    }
    for(int i=0; i<drawerp1.size() && city;i++){
        if(drawerp1[i]->is_city())
            city=false;
    }
    for(int i=0; i<drawerp2.size() && city;i++){
        if(drawerp2[i]->is_city())
            city=false;
    }
    string actual_board=this->stinguify();
    cout<<"Stringified: "<<actual_board<<endl;
    int jogador=2;
    if(player1){
        jogador=1;
    }
    if(select<0 && city){ 
        cout<<"selecionada: "<<index<<endl;
        for (int i=0; i<drawerp1.size(); i++) {
            if(drawerp1[i]->getId()==index){
                select=index;
                sx=-1;
                sy=i;
                return 1;
            }
        }
        for (int i=0; i<drawerp2.size(); i++) {
            if(drawerp2[i]->getId()==index){
                select=index;
                sx=-2;
                sy=i;
                return 1;
            }
        }

        for (int i=0; i<this->board.size(); i++) {
        for (int j=0; j<this->board[i].size(); j++) {
            if(this->board[i][j]!=NULL){
                if(this->board[i][j]->getId()==index && !this->board[i][j]->is_city() && this->board[i][j]->getPlayer()==jogador){
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
        
        if(sx>0){
        for (int i=0; i<this->board.size(); i++) {
            for (int j=0; j<this->board[i].size(); j++) {
                if(this->board[i][j]!=NULL){
                    if(this->board[i][j]->getId()==index){
                        /*select=index;
                        sx=j;
                        sy=i;
                        return 1;*/
                        
                        if(!city){
                            string msg=this->strexecutamov(jogador, 1, j+1, -1,-1, -1);
                            cout<<"execut: "<<msg<<endl;
                            select=-1;
                            string received_board=sock->innerfunc(sock->sendandreceive(msg));
                            if(received_board==actual_board){
                                cout<<"fail!!"<<endl;
                                select=-1;
                                return 1;
                            }else{
                                contagem++;
                            }
                            if (contagem>=2) {
                                city=true;
                            }
                            this->settabuleiro(sock->slitarray(received_board));
                        }else{
                            string msg=this->strexecutamov(this->board[sy][sx]->getPlayer(), 2,  sy+1, sx+1, i+1,j+1);
                            cout<<"execut: "<<msg<<endl;
                            select=-1;
                            string received_board=sock->innerfunc(sock->sendandreceive(msg));
                            if(received_board==actual_board){
                                cout<<"fail!!"<<endl;
                                select=-1;
                                return 1;
                            }
                            this->settabuleiro(sock->slitarray(received_board));    
                        }
                        player1=!player1;
                        return 2;

                        
                    }
                }
            }
        }
        }
        
        
        
        int max=this->n_vertices*this->n_vertices+1;
        for (int i=0; i<this->board.size(); i++) {
            for (int j=0; j<this->board[i].size(); j++) {
                //cout<<"Max: "<<max<<endl;
                    if(max==index){
                        
                        
                        //this->board[i][j]=this->board[sy][sx];
                        //this->board[sy][sx]=NULL;
                        
                        if(!city){
                            string msg=this->strexecutamov(jogador, 1, j+1, -1,-1, -1);
                            cout<<"execut: "<<msg<<endl;
                            select=-1;
                            string received_board=sock->innerfunc(sock->sendandreceive(msg));
                            if(received_board==actual_board){
                                cout<<"fail!!"<<endl;
                                select=-1;
                                return 1;
                            }else{
                                contagem++;
                            }
                            if (contagem>=2) {
                                city=true;
                            }
                            this->settabuleiro(sock->slitarray(received_board));
                        }else{
                        string msg=this->strexecutamov(this->board[sy][sx]->getPlayer(), 1,  sy+1, sx+1, i+1,j+1);
                        cout<<"execut: "<<msg<<endl;
                        select=-1;
                        string received_board=sock->innerfunc(sock->sendandreceive(msg));
                            if(received_board==actual_board){
                                cout<<"fail!!"<<endl;
                                select=-1;
                                return 1;
                            }
                         this->settabuleiro(sock->slitarray(received_board));    
                        }
                        player1=!player1;
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
    for(int i=0; i<this->drawerp1.size();i++){
        glPushMatrix();
        moveto(-1, i);
        /*if(this->board[i][j]->getId()==select){
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
            
        }*/
        
        this->drawerp1[i]->draw(mode);
        glPopMatrix();

    }
    for(int i=0; i<this->drawerp2.size();i++){
        glPushMatrix();
        moveto(-2, i);
        /*if(this->board[i][j]->getId()==select){
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
         
         }*/
        
        this->drawerp2[i]->draw(mode);
        glPopMatrix();

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
    if(this->movi.size()!=0){
        for (int i=0; i<movi.size(); i++) {
            if(movi[i].finj>=0)
            this->board[movi[i].fini][movi[i].finj]->set_hidden(true);
            glPushMatrix();
            float dx=movi[i].finpos.first-movi[i].inipos.first;
            float dy=movi[i].finpos.second-movi[i].inipos.second;
            //cout<<"dx: "<<dx<<" dy "<<dy<<endl;
            if(dx>0.1){
                dx=1;
            }else{if(dx<-0.1)
                dx=-1;
            else
                dx=0;}
            if(dy>0.1){
                dy=1;
            }else{
                if(dy<-0.1)
                    dy=-1;
                else
                    dy=0;
            }
            if(dx==0 && dy==0){
                glTranslated(movi[i].inipos.first, movi[i].altura, movi[i].inipos.second);
                movi[i].ptr->draw(mode);
                if(movi[i].altura>1){
                    movi[i].altura-=0.1;
                }else{
                    cout<<"delete "<<movi[i].altura<<endl;
                    if(movi[i].finj>=0){
                        this->board[movi[i].fini][movi[i].finj]->set_hidden(false);
                        delete movi[i].ptr;
                    }
                    else
                        if (movi[i].finj==-2) {
                            this->drawerp2.push_back( movi[i].ptr);
                        }else{
                            this->drawerp1.push_back( movi[i].ptr);
                        }
                    
                    movi.erase(movi.begin()+i);
                    if(filme){
                        this->frameready=true;
                        //glutTimerFunc	(	500 , this->nextframe, 0);
                    }
                    
                }
            }else{
            glTranslated(movi[i].inipos.first, movi[i].altura, movi[i].inipos.second);
                movi[i].ptr->draw(mode);
                if(movi[i].altura<3){
                    movi[i].altura+=0.1;
                }else{
                        
                    movi[i].inipos.first+=dx*0.1;
                    movi[i].inipos.second+=dy*0.1;}}
            glPopMatrix();
        }
    }else{
        this->frameready=true;
    }
    glPopMatrix();
    if(this->filme && this->frameready){
        return 3;
    
    }
    return 0;

};