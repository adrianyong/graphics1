#include <windows.h>		// Header File For Windows
//#include <gl\gl.h>			// Header File For The OpenGL32 Library
//#include <gl\glu.h>			// Header File For The GLu32 Library
#include "Image_Loading/nvImage.h"
#include <math.h>
#include <iostream>
#include <MMsystem.h>
#include <string>			// used for strings
#include <sstream>			// used for streaming text
#include "OBB.h"			//Header file for the OBB collision detection
#include "car.h"			//Header file for the car
#include "AICar.h"			//Header file for the AI car
#include "map.h"			//Header file for the map

using namespace std;



//includes areas for keyboard control, mouse control, resizing the window
//and draws a spinning rectangle

const float PI = 3.1415926535897932384626433832795f;

int	mouse_x = 0, mouse_y = 0;
bool LeftPressed = false;
int screenWidth = 1920, screenHeight = 1080;
bool keys[256];
float spin = 0;
bool isCollide = false;
bool debugBool = false;



//map variables
Map* map = new Map();
GLuint bgTexture = 0;

//time variables
double deltaT = 0;
__int64 prevTime = 0;
double timerFreqRecip = 0.000003;

//car variables
Car* car = new Car(map->Xstart, map->Ystart, map->Rstart);
AICar* car2 = new AICar(map->Xstart, map->Ystart + 10, map->Rstart);
float carRotate = 0.0f, carAcel = 0.0f, carDecel = 0.0f, AIcarRotate = 0.0f, carBoost = 0.0f, carReverse = 0.0f;

//test variables
OBB testBox;
float testMatrix[16];

//Font Variables
HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Context
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application
GLuint	base;				// Base Display List For The Font Set
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLvoid BuildFont(GLvoid)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(-30,							// Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		FW_NORMAL,						// Font Weight
		FALSE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE | DEFAULT_PITCH,		// Family And Pitch
		"Arial");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

GLvoid KillFont(GLvoid)									// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

GLvoid glPrint(string fmt)					// Custom GL "Print" Routine
{
	stringstream stream;
	stream << fmt << " " << spin;

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
														// first 32 chars not used
	glCallLists(stream.str().size(), GL_UNSIGNED_BYTE, stream.str().c_str());	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}



////draw circle function
//void drawCircle(float x, float y, float radius); //prototype the draw function

												 //OPENGL FUNCTION PROTOTYPES
//void display();				//draws everything to the screen
//void reshape(int width, int height);//called when the window is resized
//void init();				//called in winmain when the program starts.
//void processKeys();			//called in winmain to process keyboard controls
//void update();				//updates frames n stuff
							/*************    START OF OPENGL FUNCTIONS   ****************/



void display(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//cout << "Speed: " << car->speed << endl;
	glLoadIdentity();

	//draw background
	map->drawBG();
	//test map
	map->drawMap();


	//map centre
	map->drawInsideMap();
	map->debugBoxes();

	//draw finish line
	map->drawFinishLine();

	//draw car
	car->drawCar();
	car2->drawCar();

	//draw Font


	//transform hitboxes
	map->outsideMapLeftHB.transformPoints(map->matrixMapL);
	map->outsideMapRightHB.transformPoints(map->matrixMapR);
	map->outsideMapBotHB.transformPoints(map->matrixMapB);
	map->outsideMapTopHB.transformPoints(map->matrixMapT);

	map->insideMapLeftHB.transformPoints(map->matrixInL);
	map->insideMapRightHB.transformPoints(map->matrixInR);
	map->insideMapBotHB.transformPoints(map->matrixInB);
	map->insideMapTopHB.transformPoints(map->matrixInT);

	//corners
	map->outsideCornerTLHB.transformPoints(map->matrixCornerTL);
	map->outsideCornerBLHB.transformPoints(map->matrixCornerBL);
	map->outsideCornerBRHB.transformPoints(map->matrixCornerBR);
	map->outsideCornerTRHB.transformPoints(map->matrixCornerTR);

	map->insideCornerTLHB.transformPoints(map->matrixInCornerTL);
	map->insideCornerBLHB.transformPoints(map->matrixInCornerBL);
	map->insideCornerBRHB.transformPoints(map->matrixInCornerBR);
	map->insideCornerTRHB.transformPoints(map->matrixInCornerTR);

	car->carBox.transformPoints(car->matrixCar);
	car2->aicarBox.transformPoints(car2->matrixCarAI);

	testBox.transformPoints(testMatrix);

	if (debugBool)
	{
		car->drawHitBox();
		car2->drawHitBox();
		map->drawHitBox();
		map->drawP1();
		map->drawP2();
		map->drawP3();
		map->drawP4();
		map->drawP5();
		map->drawP6();
		map->drawP7();
		map->drawP8();
	}

	//movement code
	car->Xcar -= car->speed * sinf(car->Rcar * PI / 180);
	car->Ycar += car->speed * cosf(car->Rcar * PI / 180);

	//lap counters
	if (car->Xcar <= map->XP1 && car->Ycar <= map->YP1 && !car->point4Located)
	{
		cout << "Point 1 Located" << endl;
		car->point1Located = true;
	}
	if (car->Xcar <= map->XP4 && car->Ycar <= map->YP4 && car->point1Located)
	{
		cout << "Point 2 Located" << endl;
		car->point2Located = true;
	}
	if (car->Xcar >= map->XP6 && car->Ycar <= map->YP6 && car->point2Located)
	{
		cout << "Point 3 Located" << endl;
		car->point3Located = true;
	}
	if (car->Xcar <= map->XP8 && car->Ycar >= map->YP8 && car->point3Located)
	{
		cout << "Point 4 Located" << endl;
		car->point4Located = true;
	}
	if (car->Xcar == map->Xstart && map->Yinsidecord < car->Ycar <= map->Ycord &&
		car->point1Located && car->point2Located &&
		car->point3Located && car->point4Located)
	{
		map->lapCounter++;
		car->lapReset();
	}
	cout << "LAP COUNTER: " << map->lapCounter << endl;

	


	//AI Car movement hard coded
	
	
	//car to point 1
	if (car2->Xcar >= map->XP1 && !car2->point1Located)
	{

		if (car2->Ycar >= map->YP1)
		{
			car2->Rcar = 98.0f;
			car2->Ycar += car2->speed * cosf(car2->Rcar * PI / 180);
		}
		else if (car2->Ycar == map->YP1)
		{
			car2->Rcar = 90.0f;
			car2->Ycar += car2->speed * cosf(car2->Rcar * PI / 180);
		}
		
		
		car2->Xcar -= car2->speed * sinf(car2->Rcar * PI / 180);


		if (car2->Xcar <= map->XP1 && car2->Ycar <= map->YP1)
		{
			cout << "Point 1 Located" << endl;
			car2->point1Located = true;
		}
	}

	cout << car2->point1Located << endl;
	//point 1 to point 2
	if (car2->point1Located)
	{
		cout << car2->point1Located << endl;
		if (car2->Ycar >= map->YP2)
		{
			car2->Rcar = 130;
		}

		car2->Ycar += car2->speed * cosf(car2->Rcar * PI / 180);
		car2->Xcar -= car2->speed * sinf(car2->Rcar * PI / 180);

		if (car2->Xcar >= map->XP2 && car2->Ycar <= map->YP2)
		{
			cout << "Point 2 Located" << endl;
			car2->point2Located = true;
		}
	}
	//point 2 to point 3

	if (car2->point2Located)
	{
		cout << car2->point2Located << endl;
		if(car2->Rcar >= map->YP3)
		{
			car2->Rcar = 180;
		}
		
		car2->Ycar += car2->speed * cosf(car2->Rcar * PI / 180);
		car2->Xcar -= car2->speed * sinf(car2->Rcar * PI / 180);

		if (car2->Ycar <= map->YP3)
		{
			cout << "Point 3 Located" << endl;
			car2->point3Located = true;
		}
	}

	//point 3 to point4

	if (car2->point3Located)
	{

		cout << car2->point3Located << endl;
		if (car2->Xcar <= map->XP4)
		{
			car2->Rcar = 255;
		}
		car2->Ycar += car2->speed * cosf(car2->Rcar * PI / 180);
		car2->Xcar -= car2->speed * sinf(car2->Rcar * PI / 180);

		if (car2->Xcar >= map->XP4 &&  car2->Ycar <= map->YP4)
		{
			cout << "Point 4 Located" << endl;
			car2->point4Located = true;
		}
	}
	//point 4 to point5

	if (car2->point4Located)
	{

		cout << car2->point4Located << endl;
		if(car2->Xcar <= map->XP5)
		{
			car2->Rcar = 270;
		}

		car2->Xcar -= car2->speed * sinf(car2->Rcar * PI / 180);
		car2->Ycar = -560.0f;
		

		if (car2->Xcar >= map->XP5 && car2->Ycar <= map->YP5)
		{
			cout << "Point 5 Located" << endl;
			car2->point5Located = true;
		}
	}

	//point 5 to point6

	if (car2->point5Located)
	{

		cout << car2->point5Located << endl;

		car2->Rcar = 340.0f;
		/*cout << "CarX: " << car2->Xcar << endl;
		cout << "CarY: " << car2->Ycar << endl;*/
		car2->Ycar += car2->speed * 80.0f;
		car2->Xcar -= car2->speed * sinf(car2->Rcar * PI / 180);

		if (car2->Xcar >= map->XP6 && car2->Ycar >= map->YP6)
		{
			cout << "Point 6 Located" << endl;
			car2->point6Located = true;
		}
	}


	//point 6 to point7

	if (car2->point6Located)
	{

		cout << car2->point6Located << endl;
		if (car2->Ycar >= map->YP2)
		{
			car2->Rcar = 0.0f;
		}
		car2->Ycar += car2->speed * cosf(car2->Rcar * PI / 180);
		if (car2->Xcar >= map->XP7 && car2->Ycar <= map->YP7)
		{
			cout << "Point 7 Located" << endl;
			car2->point7Located = true;
		}
	}
	
	//point 7 to point8

	if (car2->point7Located)
	{

		cout << car2->point7Located << endl;
		if (car2->Ycar >= map->YP2)
		{
			car2->Rcar = 130.0f;
		}
		car2->Ycar += car2->speed * cosf(car2->Rcar * PI / 180);
		car2->Xcar -= car2->speed * sinf(car2->Rcar * PI / 180);
		if (car2->Xcar >= map->XP8 && car2->Ycar <= map->YP8)
		{
			cout << "Point 8 Located" << endl;
			car2->point8Located = true;
		}
	}

	//point 8 to finish

	if (car2->point8Located)
	{

		cout << car2->point8Located << endl;
		if (car2->Ycar >= map->YP2)
		{
			car2->Rcar = 130.0f;
		}
		car2->Ycar += car2->speed * cosf(car2->Rcar * PI / 180);
		car2->Xcar -= car2->speed * sinf(car2->Rcar * PI / 180);
		if (car2->Xcar >= map->XP2 && car2->Ycar <= map->YP2)
		{
			cout << "Lap Finished" << endl;
			car2->lapFinished = true;
		}
		
	}
	if (car2->lapFinished) {car2->lapReset();}

	



	/** PLAYER COLLISION DETECTION **/
	//top box collisions
	if (car->carBox.SAT2D(map->outsideMapTopHB) || car->carBox.SAT2D(map->insideMapBotHB))
	{
		glColor3f(1.0, 0.0, 1.0);
		isCollide = true;
		
		car->speed = 0;
		car->Ycar += -0.1f * abs(cosf(car->Rcar * PI / 180));

	}
	//top left collisions
	else if (car->carBox.SAT2D(map->outsideCornerTLHB) || car->carBox.SAT2D(map->insideCornerBRHB))
	{
		glColor3f(1.0, 0.0, 1.0);
		isCollide = true;

		car->speed = 0;
		car->Xcar += 0.1f * abs(sinf(car->Rcar * PI / 180));
		car->Ycar += -0.1f * abs(cosf(car->Rcar * PI / 180));

	}
	//left box collisions
	else if(car->carBox.SAT2D(map->outsideMapLeftHB) || car->carBox.SAT2D(map->insideMapRightHB))
	{
		glColor3f(1.0, 0.0, 1.0);
		isCollide = true;

		car->speed = 0;
		car->Xcar += 0.1f * abs(sinf(car->Rcar * PI / 180));

	}
	//bottom left box collisions
	else if (car->carBox.SAT2D(map->outsideCornerBLHB) || car->carBox.SAT2D(map->insideCornerTRHB))
	{
		glColor3f(1.0, 0.0, 1.0);
		isCollide = true;

		car->speed = 0;

		car->Xcar += 0.1f * abs(sinf(car->Rcar * PI / 180));
		car->Ycar += 0.1f * abs(cosf(car->Rcar * PI / 180));

	}
	//bottom box collisions
	else if (car->carBox.SAT2D(map->outsideMapBotHB) || car->carBox.SAT2D(map->insideMapTopHB))
	{
		glColor3f(1.0, 0.0, 1.0);
		isCollide = true;

		car->speed = 0;

 
		car->Ycar += 0.1f * abs(cosf(car->Rcar * PI / 180));

		

	}
	//bottom right box collisions
	else if (car->carBox.SAT2D(map->outsideCornerBRHB) || car->carBox.SAT2D(map->insideCornerTLHB))
	{
		glColor3f(1.0, 0.0, 1.0);
		isCollide = true;

		car->speed = 0;
		car->Xcar -= 0.1f * abs(sinf(car->Rcar * PI / 180));
		car->Ycar += 0.1f * abs(cosf(car->Rcar * PI / 180));
	}
	//right box collisions
	else if (car->carBox.SAT2D(map->outsideMapRightHB) || car->carBox.SAT2D(map->insideMapLeftHB))
	{
		glColor3f(1.0, 0.0, 1.0);
		isCollide = true;

		car->speed = 0;
		car->Xcar -= 0.1f * abs(sinf(car->Rcar * PI / 180));
		

	}
	//top right box collisions
	else if(car->carBox.SAT2D(map->outsideCornerTRHB) || car->carBox.SAT2D(map->insideCornerBLHB))
	{
		glColor3f(1.0, 0.0, 1.0);
		isCollide = true;

		car->speed = 0;
		car->Xcar -= 0.1f * abs(sinf(car->Rcar * PI / 180));
		car->Ycar -= 0.1f * abs(cosf(car->Rcar * PI / 180));
	}
	else
	{
		glColor3f(0.0, 0.0, 1.0);
		isCollide = false;
	}

	if (car->carBox.SAT2D(car2->aicarBox))
	{
		car->speed = 0;
		if (car->Xcar <= car2->Xcar)
		{
			car->Xcar -= 0.2f * abs(sinf(car->Rcar * PI / 180));
			car->Ycar -= 0.2f * abs(cosf(car->Rcar * PI / 180));
		}
		else if (car->Xcar >= car2->Xcar)
		{
			car->Xcar += 0.2f * abs(sinf(car->Rcar * PI / 180));
			car->Ycar -= 0.2f * abs(cosf(car->Rcar * PI / 180));
		}
		else if (car->Ycar >= car2->Ycar)
		{
			car->Xcar += 0.2f * abs(sinf(car->Rcar * PI / 180));
			car->Ycar -= 0.2f * abs(cosf(car->Rcar * PI / 180));
		}
		else if (car->Ycar <= car2->Ycar)
		{
			car->Xcar += 0.2f * abs(sinf(car->Rcar * PI / 180));
			car->Ycar -= 0.2f * abs(cosf(car->Rcar * PI / 180));
		}
	}

	/*---------------END OF PLAYER COLLISIONS----------------*/

	/*AI COLLISIONS*/
	//top box collisions
	if (car2->aicarBox.SAT2D(map->outsideMapTopHB) || car2->aicarBox.SAT2D(map->insideMapBotHB))
	{
		glColor3f(1.0, 0.0, 1.0);
	

		car2->speed = 0;
		car2->Ycar += -0.1f * abs(cosf(car2->Rcar * PI / 180));

	}
	//top left collisions
	else if (car2->aicarBox.SAT2D(map->outsideCornerTLHB) || car2->aicarBox.SAT2D(map->insideCornerBRHB))
	{
		glColor3f(1.0, 0.0, 1.0);


		//car2->speed = 0;
		car2->Xcar += 0.1f * abs(sinf(car2->Rcar * PI / 180));
		car2->Ycar += -0.1f * abs(cosf(car2->Rcar * PI / 180));

	}
	//left box collisions
	else if (car2->aicarBox.SAT2D(map->outsideMapLeftHB) || car2->aicarBox.SAT2D(map->insideMapRightHB))
	{
		glColor3f(1.0, 0.0, 1.0);


		//car2->speed = 0;
		car2->Xcar += 0.1f * abs(sinf(car2->Rcar * PI / 180));

	}
	//bottom left box collisions
	else if (car2->aicarBox.SAT2D(map->outsideCornerBLHB) || car2->aicarBox.SAT2D(map->insideCornerTRHB))
	{
		glColor3f(1.0, 0.0, 1.0);
		car2->speed = 0;

		car2->Xcar += 0.1f * abs(sinf(car2->Rcar * PI / 180));
		car2->Ycar += 0.1f * abs(cosf(car->Rcar * PI / 180));
		car2->Rcar += 20.0f;
	}
	//bottom box collisions
	else if (car2->aicarBox.SAT2D(map->outsideMapBotHB) || car2->aicarBox.SAT2D(map->insideMapTopHB))
	{
		glColor3f(1.0, 0.0, 1.0);


		car2->speed = 0;
		car2->Ycar += 10.0f * abs(cosf(car2->Rcar * PI / 180));
		if (car2->Rcar == 90 || car2->Rcar == 270)
		{
			car2->Ycar += 10.0f;
		}



	}
	//bottom right box collisions
	else if (car2->aicarBox.SAT2D(map->outsideCornerBRHB) || car2->aicarBox.SAT2D(map->insideCornerTLHB))
	{
		glColor3f(1.0, 0.0, 1.0);

		car2->speed = 0;
		car2->Xcar -= 0.1f * abs(sinf(car2->Rcar * PI / 180));
		car2->Ycar += 0.1f * abs(cosf(car2->Rcar * PI / 180));
		car2->Rcar += 20.0f;
	}
	//right box collisions
	else if (car2->aicarBox.SAT2D(map->outsideMapRightHB) || car2->aicarBox.SAT2D(map->insideMapLeftHB))
	{
		glColor3f(1.0, 0.0, 1.0);


		car2->speed = 0;
		car2->Xcar -= 0.1f * abs(sinf(car2->Rcar * PI / 180));


	}
	//top right box collisions
	else if (car2->aicarBox.SAT2D(map->outsideCornerTRHB) || car2->aicarBox.SAT2D(map->insideCornerBLHB))
	{
		glColor3f(1.0, 0.0, 1.0);


		car2->speed = 0;
		car2->Xcar -= 0.1f * abs(sinf(car->Rcar * PI / 180));
		car2->Ycar -= 0.1f * abs(cosf(car2->Rcar * PI / 180));
	}
	else
	{
		glColor3f(0.0, 0.0, 1.0);
	}

	

	glFlush();

	//spin += 1.0f;
	//if (spin>360)
	//	spin = 0;

	
}


void update()
{
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	__int64 currentTime = t.QuadPart;
	__int64 ticksDone = currentTime - prevTime;
	deltaT = double(ticksDone) * timerFreqRecip;
	prevTime = currentTime;

	/*define car variables with time*/

	carAcel = car->acceleration * deltaT;
	car->speed += carAcel;
	carRotate = car->rotate*deltaT;
	AIcarRotate = car2->rotate*deltaT;
	car2->speed = 0.05f;
	carDecel = car->deceleration * deltaT;
	carBoost = car->boost * deltaT;
	carReverse = car->reverse * deltaT;
}


//void drawCircle(float x, float y, float radius)
//{
//	glBegin(GL_LINE_LOOP);
//	for (float i = 0; i<360; i += 5)
//	{
//		float xcoord = x + radius * cosf(i*(PI / 180.0f));
//		float ycoord = y + radius * sinf(i*(PI / 180.0f));
//		glVertex2f(xcoord, ycoord);
//	}
//	glEnd();
//}



void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width; screenHeight = height;           // to ensure the mouse coordinates match 
														  // we will use these values to set the coordinate system

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	gluOrtho2D(car->Xcar - (width / 20), car->Xcar + (width / 20), car->Ycar - (height / 20), car->Ycar + (height / 20)); // set the coordinate system for the window


	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Mdelview Matrix
}

void init()
{
	//glClearColor(0.0, 0.0, 0.0, 0.0);						//sets the clear colour to yellow
															//glClear(GL_COLOR_BUFFER_BIT) in the display function
															//will clear the buffer to this colour.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		//enable transparency
	glEnable(GL_BLEND);

	PlaySound("bg1.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

	////define car hitbox
	car->defineHitBox();
	car2->defineHitBox();
	
	//define maps hitbox
	map->defineMapTopHB();
	map->defineMapLeftHB();
	map->defineMapBotHB();
	map->defineMapRightHB();

	map->defineInsideTopHB();
	map->defineInsideLeftHB();
	map->defineInsideBotHB();
	map->defineInsideRightHB();

	map->defineCornerTLHB();
	map->defineCornerBLHB();
	map->defineCornerBRHB();
	map->defineCornerTRHB();

	map->defineInsideCornerTLHB();
	map->defineInsideCornerBLHB();
	map->defineInsideCornerBRHB();
	map->defineInsideCornerTRHB();


	//define map texture
	map->init();

	//define car texture
	car->init();
	car2->init();

	BuildFont();
}

void processKeys()
{
	
	if (keys[VK_LEFT])
	{
		/*float speed = car->rotate*deltaT;*/
		if (isCollide && (car->Rcar >= 90.0f))
		{ 
			car->Rcar -= carRotate;
		}
		else if (isCollide && (car->Rcar < 90.0f))
		{
			car->Rcar += carRotate;
		}
		else
		{
			car->Rcar += carRotate;
			if (car->Rcar >= 360.0f)
			{
				car->Rcar = 0;
			}
		}
		cout << "Car Rotation: " << car->Rcar << endl;

	}
	if (keys[VK_RIGHT])
	{
		
		if (isCollide && (car->Rcar <= -90.0f))
		{ 
			car->Rcar += carRotate;
		}
		else if (isCollide && (car->Rcar > 90.0f))
		{
			car->Rcar -= carRotate;
		}
		else
		{
			car->Rcar -= carRotate;
			if (car->Rcar <= -360.0f)
			{
				car->Rcar = 0;
			}
		}
		cout << "Car Rotation: " << car->Rcar << endl;
	}

	if (keys[VK_UP] && !isCollide && !keys[VK_SPACE])
	{

		if (car->speed >= 0.5f)
		{
			car->speed += carAcel * 2;
			
		}
		if (keys[VK_SPACE])
		{
			car->speed += carBoost;
		}
		else {

		car->speed += car->acceleration * deltaT;
		if (car->speed > car->speedLimit)
		{
			car->speed = car->speedLimit;
		}
		}


	}
	else if (car->speed > 0 && !isCollide && !keys[VK_UP])
	{
		car->speed -= carDecel;
		if (car->speed < 0.001f)
		{
			car->speed = 0;
		}
		
	}
	
	if (keys[VK_DOWN] && !isCollide)
	{

		car->speed += carReverse;

		if (isCollide)
		{
			car->speed = 0;
			car->Xcar -= abs(-1.0f * sinf(car->Rcar * PI / 180));
			car->Ycar += abs (-1.0f * cosf(car->Rcar * PI / 180));
		}

	}
	
	
	
	if (keys['R'])
	{
		car->carReset();
		car->speed = 0;
		car2->carReset();
		car2->speed = 0;
	}

	if (keys['X'])
	{
		debugBool = true;
	}
	if (keys['C'])
	{
		debugBool = false;
	}
}
/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variables
//HDC			hDC = NULL;		// Private GDI Device Context
//HGLRC		hRC = NULL;		// Permanent Rendering Context
//HWND		hWnd = NULL;		// Holds Our Window Handle
//HINSTANCE	hInstance;		// Holds The Instance Of The Application


							/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done = false;								// Bool Variable To Exit Loop


	AllocConsole();
	FILE *stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);

	// Create Our OpenGL Window
	if (!CreateGLWindow("OpenGL Win32 Example", screenWidth, screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while (!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				done = true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if (keys[VK_ESCAPE])
				done = true;

			processKeys();			//process keyboard

			

			display();					// Draw The Scene
			update();					// update variables
			reshape(screenWidth, screenHeight);					//update the reshape
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_CLOSE:								// Did We Receive A Close Message?
	{
		PostQuitMessage(0);						// Send A Quit Message
		return 0;								// Jump Back
	}
	break;

	case WM_SIZE:								// Resize The OpenGL Window
	{
		reshape(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;								// Jump Back
	}
	break;

	case WM_LBUTTONDOWN:
	{
		mouse_x = LOWORD(lParam);
		mouse_y = screenHeight - HIWORD(lParam);
		LeftPressed = true;
	}
	break;

	case WM_LBUTTONUP:
	{
		LeftPressed = false;
	}
	break;

	case WM_MOUSEMOVE:
	{
		mouse_x = LOWORD(lParam);
		mouse_y = screenHeight - HIWORD(lParam);
	}
	break;
	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
		keys[wParam] = true;					// If So, Mark It As TRUE
		return 0;								// Jump Back
	}
	break;
	case WM_KEYUP:								// Has A Key Been Released?
	{
		keys[wParam] = false;					// If So, Mark It As FALSE
		return 0;								// Jump Back
	}
	break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*/

bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

																	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();

	return true;									// Success
}



