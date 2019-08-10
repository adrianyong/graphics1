#ifndef CAR_H
#define	CAR_H

#include <windows.h>		// Header File For Windows
#include  "Image_Loading/nvImage.h"		//Header file for loading in textures
//#include <gl\gl.h>			// Header File For The OpenGL32 Library
//#include <gl\glu.h>			// Header File For The GLu32 Library

#include <math.h>	
#include <iostream>
#include "OBB.h"			//Header file for the OBB collision detection


using namespace std;

class Car
{
public:
	float Xcar = 10, Ycar = 10, Rcar = 0;
	OBB carBox;
	float matrixCar[16];
	float speed = 0.0f;
	float speedLimit = 1.0f;
	float reverseLimit = -1.0f;
	float boost = 0.01f;
	float rotate = 10.0f;
	float acceleration = 0.02f;
	float deceleration = 0.08f;
	float reverse = -0.1f;
	double carWidth = 4;
	double carHeight = 8;

	bool point1Located = false;
	bool point2Located = false;
	bool point3Located = false;
	bool point4Located = false;

	Car(float Xcar, float Ycar, float Rcar);
	void drawCar();
	void defineHitBox();
	void drawHitBox();
	void carReset();
	void lapReset();
	GLuint loadTexture(char* name);
	void init();

};



#endif /* CAR_H */

