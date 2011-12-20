#include "MenuS.h"


MenuS::MenuS(void)
{
}

void MenuS::setTexture(int tex){
	tex_id = tex;
}

void MenuS::draw2(GLenum mode){
	glOrtho(0,width,0,height,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    
    glPushMatrix();
    glLoadIdentity();
    glTranslated((width-height)/2.0,0,0);
    
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,tex_id);
    glPushMatrix();
    if (mode == GL_SELECT)
		glLoadName (0);	
    glBegin(GL_POLYGON);
    glTexCoord2f(0,0);
    glVertex2d(height*0.0,height*0.0);
    glTexCoord2f(1,0);
    glVertex2d(height*1,height*0.0);
    glTexCoord2f(1,1);
    glVertex2d(height*1,height*1);
    glTexCoord2f(0,1);
    glVertex2d(height*0.0,height*1);
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    if (mode == GL_SELECT)
		glLoadName (1);	
    glTranslated(0,-30,0);
    
    glBegin(GL_POLYGON);
    glColor4f(192,192,192,255);
    glVertex2d(height*0.2,height*0.73);
    
    glVertex2d(height*0.8,height*0.73);
    
    glVertex2d(height*0.8,height*0.85);
    
    glVertex2d(height*0.2,height*0.85);
    glEnd();
    glPopMatrix();
    
    
    glPushMatrix();
    if (mode == GL_SELECT)
		glLoadName (2);	
    glTranslated(0,-90,0);
    
    glBegin(GL_POLYGON);
    glColor4f(192,192,192,255);
    glVertex2d(height*0.2,height*0.6);
    
    glVertex2d(height*0.8,height*0.6);
    
    glVertex2d(height*0.8,height*0.72);
    
    glVertex2d(height*0.2,height*0.72);
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    if (mode == GL_SELECT)
		glLoadName (3);	
    glTranslated(0,-97,0);
    glBegin(GL_POLYGON);
    
    glColor4f(192,192,192,255);
    glVertex2d(height*0.2,height*0.36);
    
    glVertex2d(height*0.8,height*0.36);
    
    glVertex2d(height*0.8,height*0.48);
    
    glVertex2d(height*0.2,height*0.48);
    glEnd();
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

}

void MenuS::draw(GLenum mode){
	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    this->draw2(mode);
    glEnable(GL_DEPTH_TEST);
    //drawStrings();
    glPopMatrix();
    
}

void MenuS::setDimensions(int width, int height){
	this->width = width;
	this->height = height;
}

void MenuS::setMenuStrings(vector<std::string> strings){
	menu_strings = strings;
}

void MenuS::drawStrings(){
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	float desired_h = (float) this->height / (float) menu_strings.size();
	int pos = height-60;
	string s;
	for(int i = 0; i<menu_strings.size(); i++){
		s = menu_strings[i];
		if(i==0){
			glColor3f(1.0,0.0,0.0);
			glRasterPos2i(width/3- 70, pos);
			for(int j = 0; j< s.size(); j++){
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[j]);}
			pos-= (desired_h - 40);
		}
		else{
			glColor3f(1.0,1.0,0.0);
			glRasterPos2i(width/2-100, pos);
			for(int j = 0; j< s.size(); j++){
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[j]);
			}
			pos-= desired_h;
		}
		
        
	}
	
	glPopMatrix();
}