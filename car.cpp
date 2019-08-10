#include <windows.h>		// Header File For Windows
//#include <gl\gl.h>			// Header File For The OpenGL32 Library
//#include <gl\glu.h>			// Header File For The GLu32 Library

#include  "Image_Loading/nvImage.h"		//Header file for loading in textures

#include <math.h>	
#include <iostream>
#include "OBB.h"			//Header file for the OBB collision detection
#include "map.h"			//Header file for the map
#include "car.h"			//Header file for the car


using namespace std;
Map* mapVar = new Map();
GLuint carTexture = 0;

Car::Car(float Xcar, float Ycar, float Rcar)
{
	this->Xcar = Xcar;
	this->Ycar = Ycar;
	this->Rcar = Rcar;

};

GLuint Car::loadTexture(char * name)
{
	// Texture loading object
	nv::Image img;

	GLuint carTextureID;

	// Return true on success
	if (img.loadImageFromFile(name))
	{
		glGenTextures(1, &carTextureID);
		glBindTexture(GL_TEXTURE_2D, carTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	}

	else
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);

	return carTextureID;
}

void Car::init()
{
	carTexture = loadTexture("textures/car.png");
}


void Car::drawCar()
{
	glPushMatrix();
		glTranslatef(Xcar, Ycar, 0.0);
		glRotatef(Rcar, 0.0f, 0.0f, 1.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, matrixCar);
		glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, carTexture);
			glBegin(GL_POLYGON);
				glColor3f(1.0, 1.0, 1.0);
				glTexCoord2f(0.0, 0.0); glVertex2f(-carWidth, -carHeight); //bottom left
				//glColor3f(1.0, 1.0, 0.0);
				glTexCoord2f(0.0, 1.0); glVertex2f(-carWidth, carHeight); //top left
				//glColor3f(1.0, 1.0, 0.0f);
				glTexCoord2f(1.0, 1.0); glVertex2f(carWidth, carHeight); //top right
				//glColor3f(0.0, 0.0, 1.0);
				glTexCoord2f(1.0, 0.0); glVertex2f(carWidth, -carHeight); //bottom right
			glEnd();
		glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
}

void Car::defineHitBox()
{
	carBox.vertOriginal[0].x = -carWidth;
	carBox.vertOriginal[0].y = -carHeight;

	carBox.vertOriginal[1].x = -carWidth;
	carBox.vertOriginal[1].y = carHeight;

	carBox.vertOriginal[2].x = carWidth;
	carBox.vertOriginal[2].y = carHeight;

	carBox.vertOriginal[3].x = carWidth;
	carBox.vertOriginal[3].y = -carHeight;
}

void Car::drawHitBox()
{
	carBox.drawOBB();
}

void Car::carReset()
{
	Xcar = mapVar->Xstart;
	Ycar = mapVar->Ystart;
	Rcar = mapVar->Rstart;
}

void Car::lapReset()
{
	point1Located = false;
	point2Located = false;
	point3Located = false;
	point4Located = false;


}