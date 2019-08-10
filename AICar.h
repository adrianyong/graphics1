#pragma once
#ifndef AICAR_H
#define	AICAR_H

#include <windows.h>		// Header File For Windows
#include  "Image_Loading/nvImage.h"		//Header file for loading in textures
//#include <gl\gl.h>			// Header File For The OpenGL32 Library
//#include <gl\glu.h>			// Header File For The GLu32 Library

#include <math.h>	
#include <iostream>
#include "OBB.h"			//Header file for the OBB collision detection


using namespace std;

class AICar
{
public:
	float Xcar = 10, Ycar = 10, Rcar = 0;
	OBB aicarBox;
	float matrixCarAI[16];
	float speed = 0.0f;
	float speedLimit = 10.0f;
	float reverseLimit = -1.0f;
	float boost = 0.03f;
	float rotate = 10.0f;
	float acceleration = 0.0025f;
	float deceleration = 0.06f;
	double carWidth = 4;
	double carHeight = 8;

	//map booleans
	bool point1Located = false;
	bool point2Located = false;
	bool point3Located = false;
	bool point4Located = false;
	bool point5Located = false;
	bool point6Located = false;
	bool point7Located = false;
	bool point8Located = false;
	bool lapFinished = false;
	AICar(float Xcar, float Ycar, float Rcar);
	void drawCar();
	void defineHitBox();
	void drawHitBox();
	void lapReset();
	void carReset();
	void mapMove();
	GLuint loadTexture(char* name);
	void init();
};



#endif /* AICAR_H */

