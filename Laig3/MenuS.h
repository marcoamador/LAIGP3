#pragma once

#ifdef __APPLE__
#include <GLUI/GLUI.h>
#else
#include <gl/glui.h>
#endif

#include <string>
#include <vector>
#include <iostream>

using namespace std; 

#ifndef Laig3_menu_h
#define Laig3_menu_h

class MenuS
{
	vector<string> menu_strings;
	int tex_id;
	int height, width;
public:
	MenuS(void);
	void draw();
	void setTexture(int tex_id);
	void setMenuStrings(vector<std::string> strings);
	void drawStrings();
	void setDimensions(int,int);
};

#endif