#include <windows.h>		// Header File For Windows
//#include <gl\gl.h>			// Header File For The OpenGL32 Library
//#include <gl\glu.h>			// Header File For The GLu32 Library
#include  "Image_Loading/nvImage.h"		//Header file for loading in textures
#include <math.h>
#include <iostream>
#include "OBB.h"			//Header file for the OBB collision detection
#include "map.h"			//Header file for the map
using namespace std;

GLuint mapTexture = 0;
GLuint outsideMapTexture = 0;

Map::Map(){};


GLuint Map::loadTexture(char * name)
{
	// Texture loading object
	nv::Image img;

	GLuint mapID;

	// Return true on success
	if (img.loadImageFromFile(name))
	{
		glGenTextures(1, &mapID);
		glBindTexture(GL_TEXTURE_2D, mapID);
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

	return mapID;
}


void Map::init()
{
	mapTexture = loadTexture("textures/grid.png");
	outsideMapTexture = loadTexture("textures/background.png");
}

void Map::drawBG()
{
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, outsideMapTexture);
	glBegin(GL_POLYGON);

		glTexCoord2f(-1.0, 1.0); glVertex2f(1920, 1080);
		glTexCoord2f(1.0, 1.0); glVertex2f(-1920, 1080);
		glTexCoord2f(1.0, -1.0); glVertex2f(-1920, -1080);
		glTexCoord2f(-1.0, -1.0); glVertex2f(1920, -1080);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void Map::drawMap()
{
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mapTexture);
			glBegin(GL_POLYGON);

				glTexCoord2f(-1.0, 1.0); glVertex2f(-Xcord, Ycord-50);
				//corner 1: top left
				 /*glVertex2f(-Xcord+ 10, Ycord - 45);
				 glVertex2f(-Xcord + 20, Ycord - 40);
				 glVertex2f(-Xcord + 30, Ycord - 35);
				 glVertex2f(-Xcord + 40, Ycord - 30);
				 glVertex2f(-Xcord + 50, Ycord - 25);
				 glVertex2f(-Xcord + 60, Ycord - 20);
				 glVertex2f(-Xcord + 70, Ycord - 15);
				 glVertex2f(-Xcord + 80, Ycord - 10);
				 glVertex2f(-Xcord + 90, Ycord - 5);*/
				 glVertex2f(-Xcord + 100, Ycord);

				
				glTexCoord2f(1.0, 1.0); glVertex2f(Xcord-100, Ycord);
				//corner 2: top right
				/*glVertex2f(Xcord - 90, Ycord - 5);
				glVertex2f(Xcord - 80, Ycord - 10);
				glVertex2f(Xcord - 70, Ycord - 15);
				glVertex2f(Xcord - 60, Ycord - 20);
				glVertex2f(Xcord - 50, Ycord - 25);
				glVertex2f(Xcord - 40, Ycord - 30);
				glVertex2f(Xcord - 30, Ycord - 35);
				glVertex2f(Xcord - 20, Ycord - 40);
				glVertex2f(Xcord - 10, Ycord - 45);*/
				glVertex2f(Xcord, Ycord-50);
				
				
				glTexCoord2f(1.0, -1.0); glVertex2f(Xcord, -Ycord+50);
				//corner 3: bottom right
				/*glVertex2f(Xcord - 10,-Ycord + 45);
				glVertex2f(Xcord - 20,-Ycord + 40);
				glVertex2f(Xcord - 30,-Ycord + 35);
				glVertex2f(Xcord - 40, -Ycord + 30);
				glVertex2f(Xcord - 50, -Ycord + 25);
				glVertex2f(Xcord - 60, -Ycord + 20);
				glVertex2f(Xcord - 70, -Ycord + 15);
				glVertex2f(Xcord - 80, -Ycord + 10);
				glVertex2f(Xcord - 90, -Ycord + 5);*/
				glVertex2f(Xcord-100, -Ycord);

				glTexCoord2f(-1.0, -1.0); glVertex2f(-Xcord+100, -Ycord);
				//corner 4: bottom left
			/*	glVertex2f(-Xcord + 90, -Ycord + 5);
				glVertex2f(-Xcord + 80, -Ycord + 10);
				glVertex2f(-Xcord + 70, -Ycord + 15);
				glVertex2f(-Xcord + 60, -Ycord + 20);
				glVertex2f(-Xcord + 50, -Ycord + 25);
				glVertex2f(-Xcord + 40, -Ycord + 30);
				glVertex2f(-Xcord + 30, -Ycord + 35);
				glVertex2f(-Xcord + 20, -Ycord + 40);
				glVertex2f(-Xcord + 10, -Ycord + 45);*/
				glVertex2f(-Xcord, -Ycord + 50);

			glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Map::drawInsideMap()
{
	glPushMatrix();

	//glRotatef(spin, 0.0, 0.0, 1.0);
	
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixSquare);
	//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, insideMapTexture);
		glBegin(GL_POLYGON);
			glColor3f(1.0, 1.0, 0.0);
			glTexCoord2f(0.0, 1.0); glVertex2f(-Xinsidecord, Yinsidecord -50);
			//corner1: top left
			/*glTexCoord2f(1.0, 0.0);*/ glVertex2f(-Xinsidecord  + 100, Yinsidecord) ;
			glColor3f(1.0, 0.0, 1.0);
			glTexCoord2f(1.0, 1.0); glVertex2f(Xinsidecord-100, Yinsidecord);
			//corner2: top right
			glVertex2f(Xinsidecord, Yinsidecord - 50);
			glColor3f(0.0, 1.0, 1.0);
			glTexCoord2f(1.0, 0.0); glVertex2f(Xinsidecord, -Yinsidecord+50);
			//corner 3: bottom right
			glVertex2f(Xinsidecord - 100, -Yinsidecord);
			glColor3f(1.0, 0.0, 0.0);
			glTexCoord2f(-1.0, -1.0); glVertex2f(-Xinsidecord + 100, -Yinsidecord);
			//corner 4: bottom left
			glVertex2f(-Xinsidecord, -Yinsidecord + 50);
		glEnd();
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Map::debugBoxes()
{

	glPushMatrix();
	glColor4f(1.0, 0.0, 1.0, 0.0);

	//top
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixMapT);
	glBegin(GL_POLYGON);
	glVertex2f(-Xcord + 100, Ycord);
	glVertex2f(-Xcord + 100, Ycord + 2);
	glVertex2f(Xcord - 100, Ycord + 2);
	glVertex2f(Xcord - 100, Ycord);
	glEnd();
	glPopMatrix();

	//left
	glPushMatrix();
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixMapL);
	glBegin(GL_POLYGON);
	glVertex2f(-Xcord, Ycord - 50);
	glVertex2f(-Xcord, -Ycord + 50);
	glVertex2f(-Xcord - 2, -Ycord + 50);
	glVertex2f(-Xcord - 2, Ycord - 50);
	glEnd();
	glPopMatrix();

	//bottom
	glPushMatrix();
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixMapB);
	glBegin(GL_POLYGON);
	glVertex2f(-Xcord + 100, -Ycord);
	glVertex2f(-Xcord + 100, -Ycord - 2);
	glVertex2f(Xcord - 100, -Ycord - 2);
	glVertex2f(Xcord - 100, -Ycord);
	glEnd();
	glPopMatrix();


	//right
	glPushMatrix();
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixMapR);
	glBegin(GL_POLYGON);
	glVertex2f(Xcord, Ycord - 50);
	glVertex2f(Xcord, -Ycord + 50);
	glVertex2f(Xcord + 2, -Ycord + 50);
	glVertex2f(Xcord + 2, Ycord - 50);
	glEnd();
	glPopMatrix();



	/*CORNERS OUTSIDE*/
		//top-left
	glPushMatrix();
	glColor4f(0.0, 0.0, 1.0 ,0.0);
	glTranslatef(-Xcord, Ycord - 50, 0.0);
	glRotatef(26.0, 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixCornerTL);
	glBegin(GL_POLYGON);
		glVertex2f(0, OutCornerHeight);
		glVertex2f(OutCornerWidth, OutCornerHeight);
		glVertex2f(OutCornerWidth, 0);
		glVertex2f(0, 0);
	glEnd();
	glPopMatrix();


	//bottom-left
	glPushMatrix();
	glTranslatef(-Xcord-2, -Ycord + 48, 0.0);
	glRotatef(-26.0, 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixCornerBL);
	glBegin(GL_POLYGON);
		glVertex2f(0, OutCornerHeight);
		glVertex2f(OutCornerWidth, OutCornerHeight);
		glVertex2f(OutCornerWidth, 0);
		glVertex2f(0, 0);
	glEnd();
	glPopMatrix();


	//bottom-right
	glPushMatrix();
	glTranslatef(Xcord - 102, -Ycord-2, 0.0);
	glRotatef(26.0, 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixCornerBR);
	glBegin(GL_POLYGON);
		glVertex2f(0, OutCornerHeight);
		glVertex2f(OutCornerWidth, OutCornerHeight);
		glVertex2f(OutCornerWidth, 0);
		glVertex2f(0, 0);
	glEnd();
	glPopMatrix();


	//top-right
	glPushMatrix();
	glTranslatef(Xcord+1, Ycord - 49, 0.0);
	glRotatef(153.0, 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixCornerTR);
	glBegin(GL_POLYGON);
		glVertex2f(0, OutCornerHeight);
		glVertex2f(OutCornerWidth, OutCornerHeight);
		glVertex2f(OutCornerWidth, 0);
		glVertex2f(0, 0);
	glEnd();
	glPopMatrix();


/*INSIDE BOX*/

	//insidetop
	glPushMatrix();
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixInT);
	glColor4f(1.0, 0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2f(-Xinsidecord + 100, Yinsidecord);
		glVertex2f(-Xinsidecord + 100, Yinsidecord - 2);
		glVertex2f(Xinsidecord - 100, Yinsidecord - 2);
		glVertex2f(Xinsidecord - 100, Yinsidecord);
	glEnd();
	glPopMatrix();

	//insideleft
	glPushMatrix();
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixInL);
	glBegin(GL_POLYGON);
		glVertex2f(-Xinsidecord, Yinsidecord - 50);
		glVertex2f(-Xinsidecord, -Yinsidecord + 50);
		glVertex2f(-Xinsidecord + 2, -Yinsidecord + 50);
		glVertex2f(-Xinsidecord + 2, Yinsidecord - 50);
	glEnd();
	glPopMatrix();

	//insidebot
	glPushMatrix();
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixInB);
	glBegin(GL_POLYGON);
		glVertex2f(-Xinsidecord + 100, -Yinsidecord);
		glVertex2f(-Xinsidecord + 100, -Yinsidecord + 2);
		glVertex2f(Xinsidecord - 100, -Yinsidecord + 2);
		glVertex2f(Xinsidecord - 100, -Yinsidecord);
	glEnd();
	glPopMatrix();


	//insideright
	glPushMatrix();
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixInR);
	glBegin(GL_POLYGON);
		glVertex2f(Xinsidecord, Yinsidecord - 50);
		glVertex2f(Xinsidecord, -Yinsidecord + 50);
		glVertex2f(Xinsidecord - 2, -Yinsidecord + 50);
		glVertex2f(Xinsidecord - 2, Yinsidecord - 50);
	glEnd();
	glPopMatrix();


	/*CORNERS INSIDE*/
	//top left
	glPushMatrix();
	glColor4f(0.0, 0.0, 1.0, 0.0);
	glTranslatef(-Xinsidecord+2, Yinsidecord - 52, 0.0);
	glRotatef(27.0, 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixInCornerTL);
	glBegin(GL_POLYGON);
		glVertex2f(0, OutCornerHeight);
		glVertex2f(InCornerWidth, OutCornerHeight);
		glVertex2f(InCornerWidth, 0);
		glVertex2f(0, 0);
	glEnd();
	glPopMatrix();


	//bottom-left
	glPushMatrix();
	glTranslatef(-Xinsidecord, -Yinsidecord + 50, 0.0);
	glRotatef(-27.0, 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixInCornerBL);
	glBegin(GL_POLYGON);
		glVertex2f(0, OutCornerHeight);
		glVertex2f(InCornerWidth, OutCornerHeight);
		glVertex2f(InCornerWidth, 0);
		glVertex2f(0, 0);
	glEnd();
	glPopMatrix();


	//bottom-right
	glPushMatrix();
	glTranslatef(Xinsidecord - 100, -Yinsidecord, 0.0);
	glRotatef(26.0, 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixInCornerBR);
	glBegin(GL_POLYGON);
		glVertex2f(0, OutCornerHeight);
		glVertex2f(InCornerWidth, OutCornerHeight);
		glVertex2f(InCornerWidth, 0);
		glVertex2f(0, 0);
	glEnd();
	glPopMatrix();


	//top-right
	glPushMatrix();
	glTranslatef(Xinsidecord, Yinsidecord - 50, 0.0);
	glRotatef(153.0, 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixInCornerTR);
	glBegin(GL_POLYGON);
		glVertex2f(0, OutCornerHeight);
		glVertex2f(InCornerWidth, OutCornerHeight);
		glVertex2f(InCornerWidth, 0);
		glVertex2f(0, 0);
	glEnd();
	glPopMatrix();

}


/*** OUTSIDE BOXES ***/
void Map::defineMapLeftHB()
{
	outsideMapLeftHB.vertOriginal[0].x = -Xcord;
	outsideMapLeftHB.vertOriginal[0].y = Ycord-50;

	outsideMapLeftHB.vertOriginal[1].x = -Xcord;
	outsideMapLeftHB.vertOriginal[1].y = -Ycord+50;

	outsideMapLeftHB.vertOriginal[2].x = -Xcord - 2;
	outsideMapLeftHB.vertOriginal[2].y = -Ycord + 50;

	outsideMapLeftHB.vertOriginal[3].x = -Xcord - 2;
	outsideMapLeftHB.vertOriginal[3].y = Ycord- 50;
}

void Map::defineMapRightHB()
{
	outsideMapRightHB.vertOriginal[0].x = Xcord;
	outsideMapRightHB.vertOriginal[0].y = Ycord - 50;

	outsideMapRightHB.vertOriginal[1].x = Xcord;
	outsideMapRightHB.vertOriginal[1].y = -Ycord + 50;

	outsideMapRightHB.vertOriginal[2].x = Xcord + 2;
	outsideMapRightHB.vertOriginal[2].y = -Ycord + 50;

	outsideMapRightHB.vertOriginal[3].x = Xcord + 2;
	outsideMapRightHB.vertOriginal[3].y = Ycord - 50;


}

void Map::defineMapBotHB()
{
	outsideMapBotHB.vertOriginal[0].x = -Xcord + 100;
	outsideMapBotHB.vertOriginal[0].y = -Ycord;

	outsideMapBotHB.vertOriginal[1].x = -Xcord + 100;
	outsideMapBotHB.vertOriginal[1].y = -Ycord - 2;

	outsideMapBotHB.vertOriginal[2].x = Xcord - 100;
	outsideMapBotHB.vertOriginal[2].y = -Ycord - 2;

	outsideMapBotHB.vertOriginal[3].x = Xcord - 100;
	outsideMapBotHB.vertOriginal[3].y = -Ycord;
}

void Map::defineMapTopHB()
{
	outsideMapTopHB.vertOriginal[0].x = -Xcord+100;
	outsideMapTopHB.vertOriginal[0].y = Ycord;

	outsideMapTopHB.vertOriginal[1].x = -Xcord+100;
	outsideMapTopHB.vertOriginal[1].y = Ycord+2;

	outsideMapTopHB.vertOriginal[2].x = Xcord-100;
	outsideMapTopHB.vertOriginal[2].y = Ycord+2;

	outsideMapTopHB.vertOriginal[3].x = Xcord - 100;
	outsideMapTopHB.vertOriginal[3].y = Ycord;
}

//outside corners
void Map::defineCornerTLHB()
{
	outsideCornerTLHB.vertOriginal[0].x = 0;
	outsideCornerTLHB.vertOriginal[0].y = OutCornerHeight;

	outsideCornerTLHB.vertOriginal[1].x = OutCornerWidth;
	outsideCornerTLHB.vertOriginal[1].y = OutCornerHeight;

	outsideCornerTLHB.vertOriginal[2].x = OutCornerWidth;
	outsideCornerTLHB.vertOriginal[2].y = 0;

	outsideCornerTLHB.vertOriginal[3].x = 0;
	outsideCornerTLHB.vertOriginal[3].y = 0;
}

void Map::defineCornerBLHB()
{
	outsideCornerBLHB.vertOriginal[0].x = 0;
	outsideCornerBLHB.vertOriginal[0].y = OutCornerHeight;

	outsideCornerBLHB.vertOriginal[1].x = OutCornerWidth;
	outsideCornerBLHB.vertOriginal[1].y = OutCornerHeight;

	outsideCornerBLHB.vertOriginal[2].x = OutCornerWidth;
	outsideCornerBLHB.vertOriginal[2].y = 0;

	outsideCornerBLHB.vertOriginal[3].x = 0;
	outsideCornerBLHB.vertOriginal[3].y = 0;


}

void Map::defineCornerBRHB()
{
	outsideCornerBRHB.vertOriginal[0].x = 0;
	outsideCornerBRHB.vertOriginal[0].y = OutCornerHeight;

	outsideCornerBRHB.vertOriginal[1].x = OutCornerWidth;
	outsideCornerBRHB.vertOriginal[1].y = OutCornerHeight;

	outsideCornerBRHB.vertOriginal[2].x = OutCornerWidth;
	outsideCornerBRHB.vertOriginal[2].y = 0;

	outsideCornerBRHB.vertOriginal[3].x = 0;
	outsideCornerBRHB.vertOriginal[3].y = 0;
}

void Map::defineCornerTRHB()
{
	outsideCornerTRHB.vertOriginal[0].x = 0;
	outsideCornerTRHB.vertOriginal[0].y = OutCornerHeight;

	outsideCornerTRHB.vertOriginal[1].x = OutCornerWidth;
	outsideCornerTRHB.vertOriginal[1].y = OutCornerHeight;

	outsideCornerTRHB.vertOriginal[2].x = OutCornerWidth;
	outsideCornerTRHB.vertOriginal[2].y = 0;

	outsideCornerTRHB.vertOriginal[3].x = 0;
	outsideCornerTRHB.vertOriginal[3].y = 0;
}

	/*** INSIDE BOXES ***/
void Map::defineInsideLeftHB()
{
	insideMapLeftHB.vertOriginal[0].x = -Xinsidecord;
	insideMapLeftHB.vertOriginal[0].y = Yinsidecord - 50;

	insideMapLeftHB.vertOriginal[1].x = -Xinsidecord;
	insideMapLeftHB.vertOriginal[1].y = -Yinsidecord + 50;

	insideMapLeftHB.vertOriginal[2].x = -Xinsidecord + 2;
	insideMapLeftHB.vertOriginal[2].y = -Yinsidecord + 50;

	insideMapLeftHB.vertOriginal[3].x = -Xinsidecord + 2;
	insideMapLeftHB.vertOriginal[3].y = Yinsidecord - 50;
}

void Map::defineInsideRightHB()
{
	insideMapRightHB.vertOriginal[0].x = Xinsidecord;
	insideMapRightHB.vertOriginal[0].y = Yinsidecord - 50;

	insideMapRightHB.vertOriginal[1].x = Xinsidecord;
	insideMapRightHB.vertOriginal[1].y = -Yinsidecord + 50;

	insideMapRightHB.vertOriginal[2].x = Xinsidecord - 2;
	insideMapRightHB.vertOriginal[2].y = -Yinsidecord + 50;

	insideMapRightHB.vertOriginal[3].x = Xinsidecord - 2;
	insideMapRightHB.vertOriginal[3].y = Yinsidecord - 50;


}

void Map::defineInsideBotHB()
{
	insideMapBotHB.vertOriginal[0].x = -Xinsidecord + 100;
	insideMapBotHB.vertOriginal[0].y = -Yinsidecord;

	insideMapBotHB.vertOriginal[1].x = -Xinsidecord + 100;
	insideMapBotHB.vertOriginal[1].y = -Yinsidecord + 2;

	insideMapBotHB.vertOriginal[2].x = Xinsidecord - 100;
	insideMapBotHB.vertOriginal[2].y = -Yinsidecord + 2;

	insideMapBotHB.vertOriginal[3].x = Xinsidecord - 100;
	insideMapBotHB.vertOriginal[3].y = -Yinsidecord;
}

void Map::defineInsideTopHB()
{
	insideMapTopHB.vertOriginal[0].x = -Xinsidecord + 100;
	insideMapTopHB.vertOriginal[0].y = Yinsidecord;

	insideMapTopHB.vertOriginal[1].x = -Xinsidecord + 100;
	insideMapTopHB.vertOriginal[1].y = Yinsidecord - 2;

	insideMapTopHB.vertOriginal[2].x = Xinsidecord - 100;
	insideMapTopHB.vertOriginal[2].y = Yinsidecord - 2;

	insideMapTopHB.vertOriginal[3].x = Xinsidecord - 100;
	insideMapTopHB.vertOriginal[3].y = Yinsidecord;
}

//inside corners
void Map::defineInsideCornerTLHB()
{
	insideCornerTLHB.vertOriginal[0].x = 0;
	insideCornerTLHB.vertOriginal[0].y = OutCornerHeight;

	insideCornerTLHB.vertOriginal[1].x = InCornerWidth;
	insideCornerTLHB.vertOriginal[1].y = OutCornerHeight;

	insideCornerTLHB.vertOriginal[2].x = InCornerWidth;
	insideCornerTLHB.vertOriginal[2].y = 0;

	insideCornerTLHB.vertOriginal[3].x = 0;
	insideCornerTLHB.vertOriginal[3].y = 0;
}

void Map::defineInsideCornerBLHB()
{
	insideCornerBLHB.vertOriginal[0].x = 0;
	insideCornerBLHB.vertOriginal[0].y = OutCornerHeight;

	insideCornerBLHB.vertOriginal[1].x = InCornerWidth;
	insideCornerBLHB.vertOriginal[1].y = OutCornerHeight;

	insideCornerBLHB.vertOriginal[2].x = InCornerWidth;
	insideCornerBLHB.vertOriginal[2].y = 0;

	insideCornerBLHB.vertOriginal[3].x = 0;
	insideCornerBLHB.vertOriginal[3].y = 0;


}

void Map::defineInsideCornerBRHB()
{
	insideCornerBRHB.vertOriginal[0].x = 0;
	insideCornerBRHB.vertOriginal[0].y = OutCornerHeight;

	insideCornerBRHB.vertOriginal[1].x = InCornerWidth;
	insideCornerBRHB.vertOriginal[1].y = OutCornerHeight;

	insideCornerBRHB.vertOriginal[2].x = InCornerWidth;
	insideCornerBRHB.vertOriginal[2].y = 0;

	insideCornerBRHB.vertOriginal[3].x = 0;
	insideCornerBRHB.vertOriginal[3].y = 0;
}

void Map::defineInsideCornerTRHB()
{
	insideCornerTRHB.vertOriginal[0].x = 0;
	insideCornerTRHB.vertOriginal[0].y = OutCornerHeight;

	insideCornerTRHB.vertOriginal[1].x = InCornerWidth;
	insideCornerTRHB.vertOriginal[1].y = OutCornerHeight;

	insideCornerTRHB.vertOriginal[2].x = InCornerWidth;
	insideCornerTRHB.vertOriginal[2].y = 0;

	insideCornerTRHB.vertOriginal[3].x = 0;
	insideCornerTRHB.vertOriginal[3].y = 0;
}


void Map::drawHitBox()
{
	outsideMapTopHB.drawOBB();
	outsideMapLeftHB.drawOBB();
	outsideMapBotHB.drawOBB();
	outsideMapRightHB.drawOBB();

	insideMapTopHB.drawOBB();
	insideMapLeftHB.drawOBB();
	insideMapBotHB.drawOBB();
	insideMapRightHB.drawOBB();

	insideCornerTLHB.drawOBB();
	insideCornerBLHB.drawOBB();
	insideCornerBRHB.drawOBB();
	insideCornerTRHB.drawOBB();

	outsideCornerTLHB.drawOBB();
	outsideCornerBLHB.drawOBB();
	outsideCornerBRHB.drawOBB();
	outsideCornerTRHB.drawOBB();

}



void Map::drawFinishLine()
{
	glPushMatrix();
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_POLYGON);
			glVertex2f(Xstart, Ycord);
			glVertex2f(Xstart, Yinsidecord);
			glVertex2f(Xstart - 10, Yinsidecord);
			glVertex2f(Xstart - 10, Ycord);
		glEnd();
	glPopMatrix();

}

void Map::drawP1()
{
	glPushMatrix();
		glTranslatef(XP1, YP1, 0.0);
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POLYGON);
			glVertex2f(0, 0);
			glVertex2f(0, 10);
			glVertex2f(10, 10);
			glVertex2f(10, 0);
		glEnd();
	glPopMatrix();

}
void Map::drawP2()
{
	glPushMatrix();
	glTranslatef(XP2, YP2, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(0, 10);
	glVertex2f(10, 10);
	glVertex2f(10, 0);
	glEnd();
	glPopMatrix();

}
void Map::drawP3()
{
	glPushMatrix();
	glTranslatef(XP3, YP3, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(0, 10);
	glVertex2f(10, 10);
	glVertex2f(10, 0);
	glEnd();
	glPopMatrix();

}
void Map::drawP4()
{
	glPushMatrix();
	glTranslatef(XP4, YP4, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(0, 10);
	glVertex2f(10, 10);
	glVertex2f(10, 0);
	glEnd();
	glPopMatrix();

}
void Map::drawP5()
{
	glPushMatrix();
	glTranslatef(XP5, YP5, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(0, 10);
	glVertex2f(10, 10);
	glVertex2f(10, 0);
	glEnd();
	glPopMatrix();

}
void Map::drawP6()
{
	glPushMatrix();
	glTranslatef(XP6, YP6, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(0, 10);
	glVertex2f(10, 10);
	glVertex2f(10, 0);
	glEnd();
	glPopMatrix();

}

void Map::drawP7()
{
	glPushMatrix();
	glTranslatef(XP7, YP7, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(0, 10);
	glVertex2f(10, 10);
	glVertex2f(10, 0);
	glEnd();
	glPopMatrix();

}

void Map::drawP8()
{
	glPushMatrix();
	glTranslatef(XP8, YP8, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(0, 10);
	glVertex2f(10, 10);
	glVertex2f(10, 0);
	glEnd();
	glPopMatrix();

}