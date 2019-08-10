#include <windows.h>		// Header File For Windows
//#include <gl\gl.h>			// Header File For The OpenGL32 Library
//#include <gl\glu.h>			// Header File For The GLu32 Library

#include  "Image_Loading/nvImage.h"		//Header file for loading in textures

#include <math.h>	
#include <iostream>
#include "OBB.h"			//Header file for the OBB collision detection
#include "map.h"			//Header file for the map
#include "AIcar.h"			//Header file for the car


using namespace std;
Map* mapVarAI = new Map();
GLuint carTextureAI = 0;

AICar::AICar(float Xcar, float Ycar, float Rcar)
{
	this->Xcar = Xcar;
	this->Ycar = Ycar;
	this->Rcar = Rcar;

};

GLuint AICar::loadTexture(char * name)
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

void AICar::init()
{
	carTextureAI = loadTexture("textures/carAI.png");
}


void AICar::drawCar()
{
	glPushMatrix();
	glTranslatef(Xcar, Ycar, 0.0);
	glRotatef(Rcar, 0.0f, 0.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixCarAI);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, carTextureAI);
	glBegin(GL_POLYGON);
	glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex2f(-carWidth, -carHeight); //bottom left
		glTexCoord2f(0.0, 1.0); glVertex2f(-carWidth, carHeight); //top left
		glTexCoord2f(1.0, 1.0); glVertex2f(carWidth, carHeight); //top right
		glTexCoord2f(1.0, 0.0); glVertex2f(carWidth, -carHeight); //bottom right
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void AICar::defineHitBox()
{
	aicarBox.vertOriginal[0].x = -carWidth;
	aicarBox.vertOriginal[0].y = -carHeight;

	aicarBox.vertOriginal[1].x = -carWidth;
	aicarBox.vertOriginal[1].y = carHeight;

	aicarBox.vertOriginal[2].x = carWidth;
	aicarBox.vertOriginal[2].y = carHeight;

	aicarBox.vertOriginal[3].x = carWidth;
	aicarBox.vertOriginal[3].y = -carHeight;
}

void AICar::drawHitBox()
{
	aicarBox.drawOBB();
}

void AICar::lapReset()
{
	point1Located = false;
	point2Located = false;
	point3Located = false;
	point4Located = false;
	point5Located = false;
	point6Located = false;
	point7Located = false;
	point8Located = false;
}

void AICar::carReset()
{
	Xcar = mapVarAI->Xstart;
	Ycar = mapVarAI->Ystart + 20;
	Rcar = mapVarAI->Rstart;
	lapReset();
}

