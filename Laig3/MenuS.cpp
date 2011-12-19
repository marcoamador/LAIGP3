#include "MenuS.h"


MenuS::MenuS(void)
{
}

void MenuS::draw(){
}

void MenuS::setDimensions(int width, int height){
	this->width = width;
	this->height = height;
}

void MenuS::setMenuStrings(vector<std::string> strings){
	menu_strings = strings;
}

void MenuS::drawStrings(){
	float desired_h = (float) this->height / (float) menu_strings.size();
	int pos = height-24;
	string s;
	for(int i = 0; i<menu_strings.size(); i++){
		s = menu_strings[i];
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, width, 0, height);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_LIGHTING);
		
		if(i==0){
			glColor3f(1.0,0.0,0.0);
			glRasterPos2i(width/3- 40, pos);
		}
		else{
			glColor3f(1.0,1.0,0.0);
			glRasterPos2i(width/2-100, pos);
		}
		for(int j = 0; j< s.size(); j++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[j]);}
		glEnable(GL_LIGHTING);
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );
		pos-= desired_h;
		
	}
	
}