#ifndef MAP_H
#define	MAP_H

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include <math.h>	
#include <iostream>
#include "OBB.h"			//Header file for the OBB collision detection#pragma once
using namespace std;


class Map {
public:

	float Xcord = 1000.0f, Ycord = 600.0f, Xinsidecord = 900.0f, Yinsidecord = 550.0f;
	float matrixSquare[16];

	float OutCornerWidth = 115;
	float OutCornerHeight = 2;

	float InCornerWidth = 111;
	float InCornerHeight = 2;

	float matrixMapL[16];
	float matrixMapR[16];
	float matrixMapB[16];
	float matrixMapT[16];

	float matrixInL[16];
	float matrixInR[16];
	float matrixInB[16];
	float matrixInT[16];

	float matrixCornerTL[16];
	float matrixCornerBL[16];
	float matrixCornerBR[16];
	float matrixCornerTR[16];

	float matrixInCornerTL[16];
	float matrixInCornerBL[16];
	float matrixInCornerBR[16];
	float matrixInCornerTR[16];


	float Xstart = 500.0f, Ystart = 560.0f, Rstart = 90.0f;
	float XP1 = -870.0f, YP1= 575.0f;
	float XP2 = -950.0f, YP2 = 512.0f;
	float XP3 = -950.0f, YP3 = -520.0f;
	float XP4 = -870.0f, YP4 = -560.0f;
	float XP5 =  900.0f, YP5 = -560.0f;
	float XP6 =  950.0f, YP6 = -480.0f;
	float XP7 =  950.0f, YP7 = 500.0f;
	float XP8 = 900.0f, YP8 = 575.0f;

	Map();
	GLuint loadTexture(char * name);
	void init();
	//inside map hitboxes
	OBB insideMapLeftHB;
	OBB insideMapRightHB;
	OBB insideMapBotHB;
	OBB insideMapTopHB;

	//outside map hitboxes
	OBB outsideMapLeftHB;
	OBB outsideMapRightHB;
	OBB outsideMapBotHB;
	OBB outsideMapTopHB;

	//inside corner hitboxes
	OBB insideCornerTLHB;
	OBB insideCornerBLHB;
	OBB insideCornerBRHB;
	OBB insideCornerTRHB;

	//outside corner hitboxes
	OBB outsideCornerTLHB;
	OBB outsideCornerBLHB;
	OBB outsideCornerBRHB;
	OBB outsideCornerTRHB;

	//boost box OBB
	OBB boostBox;
	OBB slowBox;

	void drawBG();

	void drawMap();
	void drawInsideMap();

	void debugBoxes();

	//hitboxes
	void defineInsideLeftHB();
	void defineInsideRightHB();
	void defineInsideBotHB();
	void defineInsideTopHB();

	void defineMapLeftHB();
	void defineMapRightHB();
	void defineMapBotHB();
	void defineMapTopHB();
	
	//corners
	void defineCornerTLHB();
	void defineCornerBLHB();
	void defineCornerBRHB();
	void defineCornerTRHB();

	void defineInsideCornerTLHB();
	void defineInsideCornerBLHB();
	void defineInsideCornerBRHB();
	void defineInsideCornerTRHB();

	void drawHitBox();
	

	//finish line
	void drawFinishLine();
	void drawP1();
	void drawP2();
	void drawP3();
	void drawP4();
	void drawP5();
	void drawP6();
	void drawP7();
	void drawP8();


	//lap counters
	int totalLapCount = 5;
	int lapCounter = 0;
	
};


#endif