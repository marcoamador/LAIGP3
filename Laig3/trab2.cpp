// aulas.cpp : Defines the entry point for the console application.
//

#ifdef __APPLE__
#include <GLUI/GLUI.h>
#else
#include <gl/glui.h>
#endif
#include "boarditem.h"
#include <math.h>
#include <sstream>
#include "Board.h"
#include "RGBpixmap.h"
#include <iostream>
#include "pick.h"
#include "misc.h"
#include "sockets.h"
#include "Scenario.h"
#include "city.h"
#include "TimerS.h"
#include "MenuS.h"
#define DIMX 500
#define DIMY 500
#define INITIALPOS_X 100
#define INITIALPOS_Y 100

#define BUFSIZE 512


#define mesaList 1
Scenario s;
float xy_aspect;		// aspect ratio da area de visualizacao
int window_w=DIMX;
int window_h=DIMY;
GLUI_Button * play_b=NULL;
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };


// variaveis globais
int main_window;
GLUI *glui, *glui2, *glui3;

float mat1_shininess[] = {20.0}; 
float mat1_specular[] = {0.8, 0.8, 0.8, 1.0}; /* specular reflection. */
float mat1_diffuse[] = {1.0, 0.0, 0.0, 1.0}; /* diffuse reflection. */

float mat2_shininess[] = {1.0}; 
float mat2_specular[] = {0.4, 0.4, 0.4, 1.0}; /* specular reflection. */
float mat2_diffuse[] = {0.3, 0.30, 0.30, 1.0}; /* diffuse reflection. */

RGBpixmap pix[10];
GLUquadric* glQ;


// variaveis usadas para o picking
float dx=0.0, dy=0.0, dz=0.0, dxAc=0.0, dzAc=0.0;	// variaveis auxiliares de posicao
float Xini=0, Yini=0;
picking *pk;										// apontador para a classe que controla a posicao dos objectos


Board * tabuleiro=NULL;

bool changesides=false;
float angler=180;
TimerS game_timer;
MenuS main_menu;

bool in_menu=false;
GLuint vbo;
GLuint vinx;

int ai1;
int ai2;
int nfree;
int peca=10;
int city=11;
#define BUFFER_OFFSET(x)((char *)NULL+(x))
void tryai(int dummy);
void tryai1(int dummy);
void changeframe(int i){
    tabuleiro->nextframe(i);
}

void DrawMesh(unsigned short * indexes, struct vertex_struct * vertexs,int n_faces) {
    glPushMatrix();
	glRotated(90, 0, 1, 0);
    glRotated(-90,1,0,0);
    for(size_t i=0; i<n_faces*3;i=i+3){
        struct vertex_struct f1=vertexs[indexes[i]];
         struct vertex_struct f2=vertexs[indexes[i+1]];
         struct vertex_struct f3=vertexs[indexes[i+2]];
        glBegin(GL_TRIANGLES);
        glNormal3f(f1.nx, f1.ny, f1.nz);glVertex3f(f1.x,f1.y,f1.z);
        glNormal3f(f2.nx, f2.ny, f2.nz);glVertex3f(f2.x,f2.y,f2.z);
        glNormal3f(f3.nx, f3.ny, f3.nz);glVertex3f(f3.x,f3.y,f3.z);
        glEnd();
    }
    glPopMatrix();
}


void drawTimer(){
	glPushMatrix();
	std::string text=game_timer.getTime();
    
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,window_w,0,window_h,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    
    
    glBegin(GL_POLYGON);
    glColor4f(0.0,0.0,1.0,0);
    glVertex2d(0,window_h);
    glVertex2d(0,window_h-40);
    glVertex2d(70,window_h-40);
    glVertex2d(70,window_h);
    glEnd();
    
    glPushMatrix();
    glColor3f(50, 50,50);           
    glRasterPos3f(10, window_h-30, 0);
    
    for (unsigned i = 0; i < text.size(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    
}


void drawScene(GLenum mode)
{
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
    
	

    if(nfree){
        gluLookAt(0, 40, 0, 0, 0, 0, 0, 0, -1);
    }else{
        glTranslated(0.0,0.0,-30.0);
        glRotated(45.0, 1.0,0.0,0.0 );
        
        
        glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] ); 
        
        glMultMatrixf( view_rotate );
    
    }
    
    
    if(!tabuleiro->rotateview()){
        if(angler<180){
            angler++;
        }else{
            angler=180;
        }
        
    }else{
        if(angler>0){
            angler--;
        }else{
            angler=0;
        }
        
    }
    glRotated(angler, 0, 1.0, 0);
	// tabuleiro
	
	//glCallList(mesaList);
    if(angler!=0 && angler!=180){
        mode=GL_RENDER;
    
    }

    if(tabuleiro->draw(mode)==3){
        glutTimerFunc(1500, changeframe, 1);
    }
    //glTranslated(0, 1, 0);
    //glCallList(peca);
	/*// esfera 1
	if (mode == GL_SELECT)
	{
		glLoadName (4);
		glPushName (1);
	}
	// define caracteristicas do material da esfera 1
	glMaterialfv(GL_FRONT, GL_SHININESS, mat1_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat1_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat1_diffuse);

	glPushMatrix();
	if (!pk->getEstado(1))
		glTranslated(pk->getDx(1), pk->getDy(1), pk->getDz(1));
	else
		glTranslated(dx, dy, dz);
	glTranslated(0.0,1.0,0.0);
	gluSphere(glQ, 0.5, 16, 8);
	glPopMatrix();

	if (mode == GL_SELECT)
	{
		glPopName ();
	}

	// fim de esfera 1

	// esfera 2
	if (mode == GL_SELECT)
	{
		glLoadName (7);
		glPushName (6);
		glPushName (2);
	}
	glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0.0,1.0,0.0);
	glPushMatrix();
	if (!pk->getEstado(2))
		glTranslated(pk->getDx(2), pk->getDy(2), pk->getDz(2));
	else
		glTranslated(dx, dy, dz);
	glTranslated(2.5,1.0,-1.0);
	gluSphere(glQ, 0.5, 16, 8);
	glPopMatrix();

	if (mode == GL_SELECT)
	{
		glPopName ();
		glPopName ();
	}

	// fim da esfera


	// teapot
	{
		glLoadName (3);
	}
	
	glColor3f(1.0,1.0,0.0);
	glPushMatrix();
	if (!pk->getEstado(3))
		glTranslated(pk->getDx(3), pk->getDy(3), pk->getDz(3));
	else
		glTranslated(dx, dy, dz);
	glTranslated(1.0,1.0,2.0);
	glutSolidTeapot(0.80);
	
    glPopMatrix();

	// fim teapot
*/
    s.draw();
	glDisable(GL_COLOR_MATERIAL);

}

void display(void)
{
    if(!in_menu){
        if(tabuleiro->getWinner()>0){
            vector<string> menu_strings;
            menu_strings.push_back("Game Over");
            menu_strings.push_back("The Winner");
            menu_strings.push_back("is");
            stringstream ss;
            ss<<"Player "<<tabuleiro->getWinner()<<"!!!";
            menu_strings.push_back(ss.str());
            main_menu.setMenuStrings(menu_strings);
            in_menu=true;
        }
    if(tabuleiro->is_playing()){
        play_b->set_name("Playing");
        play_b->disable();
    }else if(!play_b->enabled){
        play_b->set_name("Play");
        play_b->enable();
    }
	if(tabuleiro->is_stopped()){
        if(!tabuleiro->rotateview()){
            if(tabuleiro->is_player1ai()){
                glutTimerFunc(800,tryai1 , 1);
            }
        }else if(tabuleiro->is_player2ai()){
            glutTimerFunc(800,tryai , 1);
        }
    }}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 300.0 );

    gluQuadricOrientation( glQ, GLU_OUTSIDE);
    gluQuadricDrawStyle(glQ, GLU_FILL);
    gluQuadricNormals(glQ, GLU_SMOOTH);
    gluQuadricOrientation(glQ, GLU_OUTSIDE);
    gluQuadricTexture(glQ, GL_FALSE);
	glPolygonMode(GL_FRONT, GL_FILL);

	if(!in_menu){
        drawScene(GL_RENDER);}
    else{
        main_menu.draw(GL_RENDER);}
	drawTimer();
	// swapping the buffers causes the rendering above to be shown
	glutSwapBuffers();
   
	glFlush();
}


void processHits (GLint hits, GLuint buffer[])
{
	int i;
	unsigned int j;
    GLuint nnames, *ptr;
	GLuint selected_nnames, *selected_ptr;
	unsigned int z1=0, z2=0;
	unsigned int selected_z1, selected_z2;

	pk->resetEstados();
	if (hits == 0)
		return;

    printf ("hits = %d\n", hits);
    ptr = (GLuint *) buffer;

    for (i = 0; i < hits; i++) 
	{	/*  for each hit  */
		nnames = *ptr; ptr++;
		z1 = (unsigned int)*ptr; ptr++;
		z2 = (unsigned int)*ptr; ptr++;

		if ((i == 0) || (z1 < selected_z1))
		{
			selected_nnames=nnames;
			selected_z1=z1;
			selected_z2=z2;
			selected_ptr=ptr;
		}
		
		printf ("nbr_names=%u;", nnames);
		printf (" z1=%u;", z1);
		printf (" z2=%u;", z2);
		
		printf (" quem:");
		for (j = 0; j < nnames; j++)
		{
			printf (" %d", *ptr);
			ptr++;
		}
		printf ("\n");
	}

	printf ("objecto seleccionado:");
	for (j = 0; j < selected_nnames; j++)
		{
			printf (" %d", *selected_ptr);
			selected_ptr++;
		}
	printf ("\n\n");

	selected_ptr--;
	pk->setEstado(*selected_ptr);
	dx = dxAc = pk->getDx(*selected_ptr);		// obtem dados da posicao actual do objecto
	dz = dzAc = pk->getDz(*selected_ptr);

}


/* Mouse handling */
void processMouse(int button, int state, int x, int y)
{
    GLuint selectBuf[BUFSIZE];
    GLint hits;
    GLint viewport[4];

	Xini = x*view_rotate[0] - y*view_rotate[8];
	Yini = -x*view_rotate[2] + y*view_rotate[10];

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{	
        //std::cout<<"x: "<<x<<"z: "<<y<<std::endl;
        pk->setDxyz(dx, 0.0, dz);		// o objecto seleccionado esta registado em pk
        /*cout<<"zoom: "<<sqrt(pow(obj_pos[0], 2.0)+pow(obj_pos[1], 2.0)+pow(obj_pos[2], 2.0))<<endl;
        cout<<"rotate "<<view_rotate[0]<<view_rotate[1]<<view_rotate[2]<<view_rotate[3]<<view_rotate[4]<<view_rotate[5]<<view_rotate[6]<<view_rotate[7]<<view_rotate[8]<<view_rotate[9]<<view_rotate[10]<<view_rotate[11]<<view_rotate[12]<<view_rotate[13]<<view_rotate[14]<<view_rotate[15]<<endl;
		float dxx=dx,dzz=dz;
        if(changesides){
            dxx=-dxx;
            dzz=-dzz;
        }
        if(tabuleiro->processmove(pk->getObjecto(), dxx, 0, dzz))
            changesides=!changesides;*/
        if(!in_menu){
        int jogador=1;
        if (changesides) {
            jogador=2;
        }
        if(tabuleiro->tryselect(pk->getObjecto(),jogador)==2){
       changesides=!changesides;
        }}
        cout<<"Objecto: "<<pk->getObjecto()<<endl;
        pk->resetdAc();
        
        return;
	}

	glGetIntegerv (GL_VIEWPORT, viewport);

    glSelectBuffer (BUFSIZE, selectBuf);
    (void) glRenderMode (GL_SELECT);

    // inicia processo de picking
	glInitNames();
    glPushName(-1);

    glMatrixMode (GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();
	//  create 5x5 pixel picking region near cursor location
    gluPickMatrix ((GLdouble) x, (GLdouble) (window_h - y), 5.0, 5.0, viewport);

    glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 50.0 );

	if(!in_menu)
        drawScene(GL_SELECT);
    else
        main_menu.draw(GL_SELECT);

    glMatrixMode (GL_PROJECTION);
    glPopMatrix ();
    glFlush ();

    hits = glRenderMode (GL_RENDER);
    processHits (hits, selectBuf);
	

}


void processMouseMoved(int x, int y)
{	int x1, y1;

	x1 = x*view_rotate[0] - y*view_rotate[8];
	y1 = -x*view_rotate[2] + y*view_rotate[10];
    
	dx = dxAc + (float)(x1 - Xini)/20.0;
	dz = dzAc + (float)(y1 - Yini)/20.0;
    cout<<"Peca: "<<pk->getObjecto()<<" dx: "<<dx<<" dz: "<<dz<<endl;
	// pedido de refrescamento da janela
	// glutPostRedisplay();				

}

void processPassiveMouseMoved(int x, int y)
{

	// pedido de refrescamento da janela
	glutPostRedisplay();				
}



void reshape(int w, int h)
{
	int tx, ty, tw, th;

	window_w = w;	//variaveis globais; window_h e' usado em processMouse()
	window_h = h;

	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glViewport( tx, ty, tw, th );
	// ver, nos manuais, a funcao GLUI_Master.auto_set_viewport()

	xy_aspect = (float)tw / (float)th;

	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
    cout<<(int)key<<endl;
   switch (key) {
      case 27:		// tecla de escape termina o programa
           
           exit(0);
         break;
       case 127:
           tabuleiro->go_back();
           break;
       case 13:
           tabuleiro->play();
           break;
   }
}


void myGlutIdle( void )
{
  /* According to the GLUT specification, the current window is 
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != main_window ) 
    glutSetWindow(main_window);  


  glutPostRedisplay();

  /****************************************************************/
  /*            This demonstrates GLUI::sync_live()               */
  /*   We change the value of a variable that is 'live' to some   */
  /*   control.  We then call sync_live, and the control          */
  /*   associated with that variable is automatically updated     */
  /*   with the new value.  This frees the programmer from having */
  /*   to always remember which variables are used by controls -  */
  /*   simply change whatever variables are necessary, then sync  */
  /*   the live ones all at once with a single call to sync_live  */
  /****************************************************************/

//  glui->sync_live();

}
void tick_timer(int d){
    if(!tabuleiro->is_playing())
    game_timer.increment();
    glutTimerFunc(1000, tick_timer, 0);
}


void inicializacao()
{
	float ambient[] = {0.5, 0.5, 0.5, 1.0};
	float diffuse0[] = {1.0, 1.0, 1.0, 1.0}; 
	float diffuse1[] = {0.0, 1.0, 0.0, 1.0}; 
	float position0[] = {1.0, 0.0, 1.0, 0.0}; 
	float position1[] = {50.0, 0.0, 0.0, 0.0}; /* To our right. */
	float lmodel_ambient[] = {0.0, 0.0, 0.0, 1.0}; /* Set the background ambient lighting. */

	glFrontFace(GL_CCW); /* Front faces defined using a counterclockwise rotation. */
	glDepthFunc(GL_LEQUAL); /* Por defeito e GL_LESS */
	glEnable(GL_DEPTH_TEST); /* Use a depth (z) buffer to draw only visible objects. */
	//glEnable(GL_CULL_FACE); /* Use back face culling to improve speed. */
	//glCullFace(GL_BACK); /* Cull only back faces. */
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);  // define luz ambiente

	//glEnable(GL_TEXTURE_2D);				//
	pix[0].makeCheckBoard();				// cria texturas
	pix[0].setTexture(2001);
    pix[1].readBMPFile("chao.bmp");	//pix[1].readBMPFile("textura.bmp");
	pix[1].setTexture(2002);
	pix[2].readBMPFile("building.bmp");
	pix[2].setTexture(2003);
	pix[3].readBMPFile("sky.bmp");
	pix[3].setTexture(2004);
	pix[4].readBMPFile("mesa.bmp");
	pix[4].setTexture(2005);
	pix[5].readBMPFile("metal.bmp");
	pix[5].setTexture(2006);

	// por defeito a cor e de fundo e o preto
	//glClearColor(1.0,1.0,1.0,1.0);

	/* Set the light properties */
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_POSITION, position0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	/* Which shade model to use: GL_FLAT / GL_SMOOTH. */
	glShadeModel(GL_SMOOTH);


	glQ = gluNewQuadric();
	gluQuadricOrientation(glQ, GLU_OUTSIDE);

	// display list para a mesa
	glNewList(mesaList, GL_COMPILE);
		glPushMatrix();
		paralelo(10.0,1.0,10.0);
		glPopMatrix();
	glEndList();
    
    tabuleiro= new Board(9);
    glNewList(peca, GL_COMPILE);
    DrawMesh(indexes, vertexs,FACES_COUNT);
    glEndList();
    glNewList(city, GL_COMPILE);
    DrawMesh(indexes2, vertexs2, FACES_COUNT2);
    glEndList();
    glutTimerFunc(1000, tick_timer, 0);
	main_menu.setTexture(2004);
	main_menu.setDimensions(window_w, window_h);
	vector<string> menu_strings;
	menu_strings.push_back("Cannon - The Emperors Game");
	menu_strings.push_back("Jogar Humano-Humano");
	menu_strings.push_back("Jogar Humano-CPU");
	menu_strings.push_back("Jogar CPU-CPU");
	main_menu.setMenuStrings(menu_strings);

}

void closeconnection(){
    if(tabuleiro!=NULL){
        tabuleiro->exit_sock();
    }
}

void playvideo(int dummy){
    if(tabuleiro!=NULL){
        in_menu=false;
        tabuleiro->play();
    }

}

void goback(int dummy){
    if (tabuleiro!=NULL) {
        tabuleiro->go_back();
    }

}

void tryai(int dummy){
    tabuleiro->ai_play(2, 2);
}

void tryai1(int dummy){
    tabuleiro->ai_play(1, 2);
}
void setai1(int dummy){
    tabuleiro->setAi1(ai1);
}

void setai2(int dummy){
    tabuleiro->setAi2(ai2);
}
int main(int argc, char* argv[])
{
    atexit(closeconnection);
    glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowSize (DIMX, DIMY);
   glutInitWindowPosition (INITIALPOS_X, INITIALPOS_Y);
   main_window = glutCreateWindow (argv[0]);
 
   glutDisplayFunc(display);
   GLUI_Master.set_glutReshapeFunc(reshape);
   GLUI_Master.set_glutKeyboardFunc (keyboard);
   GLUI_Master.set_glutMouseFunc(processMouse);
   glutMotionFunc(processMouseMoved);
   glutPassiveMotionFunc(processPassiveMouseMoved);   
   GLUI_Master.set_glutSpecialFunc( NULL );


	/*** Create the bottom subwindow ***/
	glui2 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM );
														/*	GLUI_SUBWINDOW_RIGHT
															GLUI_SUBWINDOW_LEFT
															GLUI_SUBWINDOW_TOP
															GLUI_SUBWINDOW_BOTTOM */
	glui2->set_main_gfx_window( main_window );

	GLUI_Rotation *view_rot = glui2->add_rotation( "Rotacao", view_rotate );
	view_rot->set_spin( 1.0 );
	glui2->add_column( false );

	
	//glui3 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM);
	//glui3->set_main_gfx_window( main_window );

	GLUI_Translation *trans_z = 
	glui2->add_translation( "Zoom", GLUI_TRANSLATION_Z, &obj_pos[2] );
	trans_z->set_speed( .02 );
    glui2->add_column( false );
    play_b=glui2->add_button("Play",-1,playvideo);
    glui2->add_button("Back",-1,goback);
    glui2->add_checkbox("AI Player1",&ai1,-1,setai1);
    glui2->add_checkbox("AI Player2",&ai2,-1,setai2);
    glui2->add_checkbox("Top view",&nfree,-1,setai2);
    //glui2->add_button("Player 1 ai",-1,tryai1);
	//glui2->add_button("Player 2 ai",-1,tryai);
	/* We register the idle callback with GLUI, not with GLUT */
	GLUI_Master.set_glutIdleFunc( myGlutIdle );
   
	inicializacao();
   
    //cout<<a.sendandreceive("initialize.\n")<<endl;
    
	// numero de objectos para picking
	pk = new picking(10*10*2);
    
	glutMainLoop();

	delete pk;

	return 0;
}
