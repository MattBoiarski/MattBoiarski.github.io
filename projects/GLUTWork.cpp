//core
#include <iostream>
#include <cstdlib>
#include <string>
#include <iomanip>

//glut
#include "C:/freeglut/include/GL/freeglut.h"
#include <GL/gl.h>
//maths & util
#include <math.h>
#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>

#include "Timer.h"


#define CORRECT_MATRIX 12
#define KEY_ESC 27
#define WIRE_THICK 50
using std::cin;
using std:: cout;
using std::string;
using std::fixed;
using std::setprecision;
using std::endl;
using std::vector;


const vector<vector<GLfloat>> face1_Matrix = {           {-0.5f,-0.5f,0.5f},
													     {0.5f,-0.5f,0.5f},
												         {0.5f,0.5f, 0.5f},
												         {-0.5f,0.5f,0.5f} };

const vector<vector<GLfloat>> face2_Matrix = {           {-0.5f,-0.5f,-0.5f},
													     {-0.5f,0.5f,-0.5f},
												         {0.5f,0.5f,-0.5f},
												         {0.5f,-0.5f,-0.5f} };

const vector<vector<GLfloat>> face3_Matrix = {           {-0.5f,-0.5f,0.5f},
													     {-0.5f,0.5f,0.5f},
												         {-0.5f,0.5f,-0.5f},
												         {-0.5f,-0.5f,-0.5f} };

const vector<vector<GLfloat>> face4_Matrix = {		     {0.5f,-0.5f,-0.5f},
													     {0.5f,0.5f,-0.5f},
													     {0.5f,0.5f,0.5f},
													     {0.5f,-0.5f,0.5f} };

const vector<vector<GLfloat>> face5_Matrix = {			 {-0.5f,0.5f,0.5f},
													     {0.5f,0.5f,0.5f},
													     {0.5f,0.5f,-0.5f},
												         {-0.5f,0.5f,-0.5f} };

const vector<vector<GLfloat>> face6_Matrix = {			 {-0.5f,-0.5f,0.5f},
													     {-0.5f,-0.5f,-0.5f},
													     {0.5f,-0.5f,-0.5f},													     {0.5f,-0.5f,0.5f} };
int 
	COUNT_MATRIX_INIT = 0,
	COUNT_matrix_MATRIX_INIT = 0,
	COUNT_FACE_INIT = 0,
	COUNT_FACE_COPY_INIT = 0,
	COUNT_CUBE_INIT = 0,
	COUNT_CUBE_COPY_INIT = 0;

enum COLORS { RED, GREEN, BLUE, ORANGE, YELLOW, WHITE };

enum DIRECTIONS { UP, DOWN, LEFT, RIGHT, /* AUX DIRECTIONS */ BACK , FORTH };
								  

bool 
	fullscreen = false,
	mouseDown = false;

GLfloat 
	xrot = 100.f,
	yrot = -100.f,
	xdiff = 100.f,
    ydiff = 100.f,
	tra_x = 0.f,
	tra_y = 0.f,
	tra_z = 0.0f;

float perspective_multiplier = 70.f; //70.f is comfortable for viewing 
GLfloat resize_factor = 1.f; //scale
GLfloat WIRE_OFFSET = 0.f;
//CPPTimer FPS_COUNT;

//Global refresh
void refresh() {
	glFlush();
	glutSwapBuffers();
}

//global Maths check
int check(const vector<vector<GLfloat>> v) {
	int check = 0;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 3; y++) {
			if (v.at(x).at(y) == 0.5f || v.at(x).at(y) == -0.5f) {
				check++;
			}
		}
	}
	return check;
}

//CLASS DEFS
class Matrixs
{ //base
public:
	//std::vector style === GLfloat[][];
	std::vector<std::vector<GLfloat>> matrix;
	
	//defualt construct
	Matrixs()
	{
		/*cout << "matrixs Constructor Called! #" << ++COUNT_MATRIX_INIT << " & ";*/
		
		matrix = { {0.5f, 0.5f ,0.5f},	//	{x,y,z}
				   {0.5f, 0.5f ,0.5f},
				   {0.5f, 0.5f ,0.5f},
				   {0.5f, 0.5f ,0.5f}, };
		
		/*if (check(matrix) == 12) {
			cout << "Passed Check!\n";
		}
		else if (check(matrix) != 12) {
			cout << "Did not pass check!\n";
		}*/
	}
	bool operator==(const Matrixs & beingCompared) {
			int check = 0;
			for (int x = 0; x < 4; x++) {
				for (int y = 0; y < 3; y++) {
					if (beingCompared.matrix.at(x).at(y) == matrix.at(x).at(y)) {
						check++;
					}
				}
			}
		return (check == CORRECT_MATRIX);
	}
	bool operator==(const std::vector<std::vector<GLfloat>> vec) {
		int check = 0;
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 3; y++) {
				if (vec.at(x).at(y) == matrix.at(x).at(y)) {
					check++;
				}
			}
		}
		if (check != CORRECT_MATRIX) { cout << "Key Val: " << CORRECT_MATRIX << " Actual Val: " << check; }
		return (check == CORRECT_MATRIX);
	}
	//debug bundle + read per xyz
	string getArrayAtString(int x)
	{
		//get
		std::string pure_x = std::to_string(arrayAt(x, 0, *this));
		std::string pure_y = std::to_string(arrayAt(x, 0, *this));
		std::string pure_z = std::to_string(arrayAt(x, 0, *this));

		std::string s = "{" + pure_x + ", " + pure_y
			+ ", " + pure_z + "}";
		return std::string(s);
	}



	void debug() {
		cout << "matrix @ " << this << " : {"
			<< this->getArrayAtString(0) << ", "
			<< this->getArrayAtString(1) << ", "
			<< this->getArrayAtString(2) << ", "
			<< this->getArrayAtString(3) << ", " << endl;
	}
	
	//get
	static GLfloat arrayAt(int x, int y, const Matrixs grp) 
	{
		return static_cast<GLfloat>(grp.matrix.at(x).at(y));
	}

	//set
	void arraySet(int x, int y, Matrixs grp, GLfloat val) 
	{
		grp.matrix.at(x).at(y) = static_cast<GLfloat>(val);
	}

	Matrixs(Matrixs& grp_Address)
	{
		/*cout << "matrixs Copy Constructor Called! #" << ++COUNT_matrix_COPY_INIT <<" & ";*/
		matrix = grp_Address.matrix;

		/*if (check(matrix) == 12) {
			cout << "Passed Check!\n";
		}
		else if (check(matrix) != 12) {
			cout << "Did not pass check!\n";
		}*/
	};
	
};

/// <summary>
/// FACET CALCULATIONS, TIES, AND DEBUG
/// </summary>

class Face { 
public:
	//dec
	Matrixs Data;
	int ColorID;
	int DirectionID;
	//def init
	Face();
	int getColorID();
	int getDirectionID();
	void setColorID(int);
	void setDirectionID(int);
	//bool isRule = false;
		
};

// Zero Param Face Contruct
Face::Face() {
	this->ColorID = -1;
	this->DirectionID = UP;
	this->Data = Matrixs(); //Each face holds a Matrixs Obj / addressable 2D Matrix of size 4x3
}
// Face get
int Face::getColorID() { return this->ColorID; }
int Face::getDirectionID() { return this->DirectionID; }

// Face set
void Face::setColorID(int color) {
	if (color <= 5 && color >= 0) { 
		this->ColorID = color; 
	} else {
		cout << "Color was " << color << "!" << endl;
		exit(-1);
	}
}

void Face::setDirectionID(int direction) {
	if (direction <= 5 && direction >= 0) {
		this->DirectionID = direction;
	} else {
		cout << "Direction was " << direction << "!" << endl;
		exit(-1);
	}
}

// 1x1 Basic Group
class Cube
{
public:
	Face
		f1,f2,f3,
		f4,f5,f6;
	//std::array<Face> FACE_OBJ = { f1,f2,f3,f4,f5,f6 };
	Face north_face; //or Rule face
	//def init
	Cube()
	{
		//cout << "Cube Constructor Called! # " << COUNT_CUBE_INIT++ /*<< " & "*/;
		f1.Data.matrix = face1_Matrix; f1.DirectionID = LEFT; f1.ColorID = GREEN;
		f2.Data.matrix = face2_Matrix; f2.DirectionID = RIGHT; f2.ColorID = BLUE;
		f3.Data.matrix = face3_Matrix; f3.DirectionID = UP; f3.ColorID = RED;
		f4.Data.matrix = face4_Matrix; f4.DirectionID = DOWN; f4.ColorID = ORANGE;
		f5.Data.matrix = face5_Matrix; f5.DirectionID = BACK; f5.ColorID = YELLOW;
		f6.Data.matrix = face6_Matrix; f6.DirectionID = FORTH; f6.ColorID = WHITE;
		this->north_face = f5;
	}
	//dec copy construct
	Cube(Cube& cCopy) 
	{
		//cout << "Cube Copy Construct Called! # " << ++COUNT_CUBE_COPY_INIT /*<< " & "*/;
		f1.Data = cCopy.f1.Data; f1.ColorID = cCopy.f1.ColorID; f1.DirectionID = cCopy.f1.DirectionID;
		f2.Data = cCopy.f2.Data; f2.ColorID = cCopy.f2.ColorID; f2.DirectionID = cCopy.f2.DirectionID;
		f3.Data = cCopy.f3.Data; f3.ColorID = cCopy.f3.ColorID; f3.DirectionID = cCopy.f3.DirectionID;
		f4.Data = cCopy.f4.Data; f4.ColorID = cCopy.f4.ColorID; f4.DirectionID = cCopy.f4.DirectionID;
		f5.Data = cCopy.f5.Data; f5.ColorID = cCopy.f5.ColorID; f5.DirectionID = cCopy.f5.DirectionID;
		f6.Data = cCopy.f6.Data; f6.ColorID = cCopy.f6.ColorID; f6.DirectionID = cCopy.f6.DirectionID;
		this->north_face = cCopy.north_face;
	}

	//SET BY FACE OBJ & BY DIRECTION & BY COLOR 
	Cube(Face FACE_1, int FACE_1_DIRECTION, int FACE_1_COLOR,
		Face FACE_2, int FACE_2_DIRECTION, int FACE_2_COLOR,
		Face FACE_3, int FACE_3_DIRECTION, int FACE_3_COLOR,
		Face FACE_4, int FACE_4_DIRECTION, int FACE_4_COLOR,
		Face FACE_5, int FACE_5_DIRECTION, int FACE_5_COLOR,
		Face FACE_6, int FACE_6_DIRECTION, int FACE_6_COLOR){

		f1.setDirectionID(FACE_1_DIRECTION); f1.setColorID(FACE_1_COLOR);
		f2.setDirectionID(FACE_2_DIRECTION); f2.setColorID(FACE_2_COLOR);
		f3.setDirectionID(FACE_3_DIRECTION); f3.setColorID(FACE_3_COLOR);
		f4.setDirectionID(FACE_4_DIRECTION); f4.setColorID(FACE_4_COLOR);
		f5.setDirectionID(FACE_5_DIRECTION); f5.setColorID(FACE_5_COLOR);
		f6.setDirectionID(FACE_6_DIRECTION); f6.setColorID(FACE_6_COLOR);

		this->north_face = f5;
	}
	Cube PToCube(){
		Cube* cP = this;
			  Cube cH(cP->f1, cP->f1.getDirectionID(), cP->f1.getColorID(),
					  cP->f2, cP->f2.getDirectionID(), cP->f2.getColorID(),
				      cP->f3, cP->f3.getDirectionID(), cP->f3.getColorID(),
					  cP->f4, cP->f4.getDirectionID(), cP->f4.getColorID(),
					  cP->f5, cP->f5.getDirectionID(), cP->f5.getColorID(),
					  cP->f6, cP->f6.getDirectionID(), cP->f6.getColorID());
		return cH;
	}

	void debug() {
		/*DEBUG LINE*/

		if (this->f1.Data.matrix == face1_Matrix) { cout << "face1 -> CORRECT\t Direction: " 
			<< f1.getDirectionID() << " & Color: " << f1.getColorID() << endl; }
		else { cout << "face1 -> CHECK -> FALSE\n"; }
		
		if (this->f2.Data.matrix == face2_Matrix) { cout << "face2 -> CORRECT\t Direction: "
				<< f2.getDirectionID() << " & Color: " << f2.getColorID() << endl;
		}
		else { cout << "face2 -> CHECK -> FALSE\n"; }
		
		if (this->f3.Data.matrix == face3_Matrix) { cout << "face3 -> CORRECT\t Direction: "
				<< f3.getDirectionID() << " & Color: " << f3.getColorID() << endl;
		}
		else { cout << "face3 -> CHECK -> FALSE\n"; }
		
		if (this->f4.Data.matrix == face4_Matrix) { cout << "face4 -> CORRECT\t Direction: "
				<< f4.getDirectionID() << " & Color: " << f4.getColorID() << endl;
		}
		else { cout << "face4 -> CHECK -> FALSE\n"; }
		
		if (this->f5.Data.matrix == face5_Matrix) { cout << "face5 -> CORRECT\t Direction: "
				<< f5.getDirectionID() << " & Color: " << f5.getColorID() << endl;
		}
		else { cout << "face5 -> CHECK -> FALSE\n"; }
		
		if (this->f6.Data.matrix == face6_Matrix) { cout << "face6 -> CORRECT\t Direction: "
				<< f6.getDirectionID() << " & Color: " << f6.getColorID() << endl;
		}
		else { cout << "face6 -> CHECK -> FALSE\n"; }
		
		
	}
	/*bool isSolved(Face, Face, Face, Face, Face, Face, Face, Face, Face);
	bool isSolved(Cube);*/
	
};

// 3x3 Grouping of Cubes
class Slice {
public:
	Cube
		c1, c2, c3, //top 
		c4, c5, c6, //mid
		c7, c8, c9; //bottom

	Cube center;
	bool isTurned;

	Slice() {
		center = c5;
		isTurned = false; //not turned to start with
	}
	bool isSolved(Slice S) {
		int key = S.center.north_face.ColorID;
		int count = 0;
		if (c1.north_face.ColorID == key) {
			count++;
		}
		if (c2.north_face.ColorID == key) {
			count++;
		}
		if (c3.north_face.ColorID == key) {
			count++;
		}
		if (c4.north_face.ColorID == key) {
			count++;
		}
		if (c6.north_face.ColorID == key) {
			count++;
		}
		if (c7.north_face.ColorID == key) {
			count++;
		}
		if (c8.north_face.ColorID == key) {
			count++;
		}
		if (c9.north_face.ColorID == key) {
			count++;
		}
		if (count == 8) {
			return true;
		}
		return false;
	}

	// void turnSlice(Slice);
};


/*
getANG FUNC{
GLfloat ang;
	switch (/* Find const */ /*) {
	case 'U':
		ang = -1.0;
		break;
	case 'D':
		ang = 1.0;
		break;
	case 'R':
		ang = -1.0;
		break;
	case 'L':
		ang = 1.0;
		break;
	case 'F':
		ang = -1.0;
		break;
	case 'B':
		ang = 1.0;
		break;
	}
	return ang;
}
*/
//void Slice::turnSlice(DIRECTIONS param) {
//	GLfloat getAng = 
//	switch (/*find const*/) {
//	case 'U':
//		axis = 2;
//		break;
//	case 'D':
//		axis = 2;
//		break;
//	case 'R':
//		axis = 1;
//		break;
//	case 'L':
//		axis = 1;
//		break;
//	case 'F':
//		axis = 0;
//		break;
//	case 'B':
//		axis = 0;
//		break;
//	}
//}

// 3x3x3 Grouping of Cubes --> (REALITY : 3-Grouping Slices) 
class Rubik {
public:
	Slice s1, s2, s3;
	Rubik() {
		Slice mid = s2;
	}
	// bool isSolved(Slice, Slice, Slice);
};

//RULES FOR RUBIK
//bool Cube::isSolved(Face f1, Face f2, Face f3, Face f4, Face f5, Face f6, Face f7, Face f8, Face f9)
//{
//	int count_C = 0, count_D = 0, key_C = f4.getColorID(), key_D = f4.getDirectionID();
//	if (f1.getColorID() == key_C) 
//	{
//		count_C++;
//		if (f1.getDirectionID() == key_D) 
//		{
//			count_D++;
//		}
//	}
//	if (f2.getColorID() == key_C) 
//	{
//		count_C++;
//		if (f2.getDirectionID() == key_D) 
//		{
//			count_D++;
//		}
//	}
//	if (f3.getColorID() == key_C) 
//	{
//		count_C++;
//		if (f3.getDirectionID() == key_D) 
//		{
//			count_D++;
//		}
//	}
//	if (f5.getColorID() == key_C) 
//	{
//		count_C++;
//		if (f5.getDirectionID() == key_D) 
//		{
//			count_D++;
//		}
//	}
//	if (f6.getColorID() == key_C)
//	{
//		count_C++;
//		if (f6.getDirectionID() == key_D)
//		{
//			count_D++;
//		}
//	}
//	if (f7.getColorID() == key_C)
//	{
//		count_C++;
//		if (f7.getDirectionID() == key_D)
//		{
//			count_D++;
//		}
//	}
//	if (f8.getColorID() == key_C)
//	{
//		count_C++;
//		if (f8.getDirectionID() == key_D) {
//			count_D++;
//		}
//	}
//	if (f9.getColorID() == key_C)
//	{
//		count_C++;
//		if (f9.getDirectionID() == key_D) 
//		{
//			count_D++;
//		}
//	}
//	if (count_C == 9 && count_D == 9) 
//	{
//		return true;
//	}
//	return false;
/*}
bool Cube::isSolved(Cube cS) {
	int count_C = 0, count_D = 0, key_C = f4.getColorID(), key_D = f4.getDirectionID();
	if (cS.f1.getColorID() == key_C)
	{
		count_C++;
		if (cS.f1.getDirectionID() == key_D)
		{
			count_D++;
		}
	}
	if (cS.f2.getColorID() == key_C)
	{
		count_C++;
		if (cS.f2.getDirectionID() == key_D)
		{
			count_D++;
		}
	}
	if (cS.f3.getColorID() == key_C)
	{
		count_C++;
		if (cS.f3.getDirectionID() == key_D)
		{
			count_D++;
		}
	}
	if (cS.f5.getColorID() == key_C)
	{
		count_C++;
		if (cS.f5.getDirectionID() == key_D)
		{
			count_D++;
		}
	}
	if (cS.f6.getColorID() == key_C)
	{
		count_C++;
		if (cS.f6.getDirectionID() == key_D)
		{
			count_D++;
		}
	}
	if (cS.f7.getColorID() == key_C)
	{
		count_C++;
		if (f7.getDirectionID() == key_D)
		{
			count_D++;
		}
	}
	if (f8.getColorID() == key_C)
	{
		count_C++;
		if (f8.getDirectionID() == key_D) {
			count_D++;
		}
	}
	if (f9.getColorID() == key_C)
	{
		count_C++;
		if (f9.getDirectionID() == key_D)
		{
			count_D++;
		}
	}
	if (count_C == 9 && count_D == 9)
	{
		return true;
	}
	return false;
}*/

// Raw function that returns a Cube object
Cube drawBox()
{
	
	glTranslatef(tra_x, tra_y, tra_z); //move
	//fast as fuck boiiii
	glEnable(GL_CULL_FACE); 
	glCullFace(GL_BACK); //idk mane rubik's cube booty just isn't relevant to render
	//init Cube
	/*Cube c;*/
	//Build
		/* Draw a simple cube. */
	Cube c;
	//LEFT
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f); //green
	glVertex3f(Matrixs::arrayAt(0, 0, c.f1.Data), Matrixs::arrayAt(0,1, c.f1.Data), Matrixs::arrayAt(0,2,c.f1.Data));
	glVertex3f(Matrixs::arrayAt(1, 0, c.f1.Data), Matrixs::arrayAt(1, 1, c.f1.Data), Matrixs::arrayAt(1, 2, c.f1.Data));
	glVertex3f(Matrixs::arrayAt(2, 0, c.f1.Data), Matrixs::arrayAt(2, 1, c.f1.Data), Matrixs::arrayAt(2, 2, c.f1.Data));
	glVertex3f(Matrixs::arrayAt(3, 0, c.f1.Data), Matrixs::arrayAt(3, 1, c.f1.Data), Matrixs::arrayAt(3, 2, c.f1.Data));
	glEnd();
	//draw wires
	glBegin(GL_LINES);
	glLineWidth((GLfloat)WIRE_THICK);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(Matrixs::arrayAt(0, 0, c.f1.Data) + WIRE_OFFSET, Matrixs::arrayAt(0, 1, c.f1.Data) + WIRE_OFFSET, Matrixs::arrayAt(0, 2, c.f1.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(1, 0, c.f1.Data) + WIRE_OFFSET, Matrixs::arrayAt(1, 1, c.f1.Data) + WIRE_OFFSET, Matrixs::arrayAt(1, 2, c.f1.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(2, 0, c.f1.Data) + WIRE_OFFSET, Matrixs::arrayAt(2, 1, c.f1.Data) + WIRE_OFFSET, Matrixs::arrayAt(2, 2, c.f1.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(3, 0, c.f1.Data) + WIRE_OFFSET, Matrixs::arrayAt(3, 1, c.f1.Data) + WIRE_OFFSET, Matrixs::arrayAt(3, 2, c.f1.Data) + WIRE_OFFSET);
	glEnd();
	/*
		Opp pair 1
	*/
	//RIGHT
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 1.0f); //blue
	glVertex3f(Matrixs::arrayAt(0, 0, c.f2.Data), Matrixs::arrayAt(0, 1, c.f2.Data), Matrixs::arrayAt(0, 2, c.f2.Data));
	glVertex3f(Matrixs::arrayAt(1, 0, c.f2.Data), Matrixs::arrayAt(1, 1, c.f2.Data), Matrixs::arrayAt(1, 2, c.f2.Data));
	glVertex3f(Matrixs::arrayAt(2, 0, c.f2.Data), Matrixs::arrayAt(2, 1, c.f2.Data), Matrixs::arrayAt(2, 2, c.f2.Data));
	glVertex3f(Matrixs::arrayAt(3, 0, c.f2.Data), Matrixs::arrayAt(3, 1, c.f2.Data), Matrixs::arrayAt(3, 2, c.f2.Data));
	glEnd();
	//draw wires
	glBegin(GL_LINES);
	glLineWidth((GLfloat)WIRE_THICK);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(Matrixs::arrayAt(0, 0, c.f2.Data) + WIRE_OFFSET, Matrixs::arrayAt(0, 1, c.f2.Data) + WIRE_OFFSET, Matrixs::arrayAt(0, 2, c.f2.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(1, 0, c.f2.Data) + WIRE_OFFSET, Matrixs::arrayAt(1, 1, c.f2.Data) + WIRE_OFFSET, Matrixs::arrayAt(1, 2, c.f2.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(2, 0, c.f2.Data) + WIRE_OFFSET, Matrixs::arrayAt(2, 1, c.f2.Data) + WIRE_OFFSET, Matrixs::arrayAt(2, 2, c.f2.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(3, 0, c.f2.Data) + WIRE_OFFSET, Matrixs::arrayAt(3, 1, c.f2.Data) + WIRE_OFFSET, Matrixs::arrayAt(3, 2, c.f2.Data) + WIRE_OFFSET);
	glEnd();

	//UP
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f); //red
	glVertex3f(Matrixs::arrayAt(0, 0, c.f3.Data), Matrixs::arrayAt(0, 1, c.f3.Data), Matrixs::arrayAt(0, 2, c.f3.Data));
	glVertex3f(Matrixs::arrayAt(1, 0, c.f3.Data), Matrixs::arrayAt(1, 1, c.f3.Data), Matrixs::arrayAt(1, 2, c.f3.Data));
	glVertex3f(Matrixs::arrayAt(2, 0, c.f3.Data), Matrixs::arrayAt(2, 1, c.f3.Data), Matrixs::arrayAt(2, 2, c.f3.Data));
	glVertex3f(Matrixs::arrayAt(3, 0, c.f3.Data), Matrixs::arrayAt(3, 1, c.f3.Data), Matrixs::arrayAt(3, 2, c.f3.Data));
	glEnd();
	//draw wires
	glBegin(GL_LINES);
	glLineWidth((GLfloat)WIRE_THICK);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(Matrixs::arrayAt(0, 0, c.f3.Data) + WIRE_OFFSET, Matrixs::arrayAt(0, 1, c.f3.Data) + WIRE_OFFSET, Matrixs::arrayAt(0, 2, c.f3.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(1, 0, c.f3.Data) + WIRE_OFFSET, Matrixs::arrayAt(1, 1, c.f3.Data) + WIRE_OFFSET, Matrixs::arrayAt(1, 2, c.f3.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(2, 0, c.f3.Data) + WIRE_OFFSET, Matrixs::arrayAt(2, 1, c.f3.Data) + WIRE_OFFSET, Matrixs::arrayAt(2, 2, c.f3.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(3, 0, c.f3.Data) + WIRE_OFFSET, Matrixs::arrayAt(3, 1, c.f3.Data) + WIRE_OFFSET, Matrixs::arrayAt(3, 2, c.f3.Data) + WIRE_OFFSET);
	glEnd();
	/*
		Opp pair 2
	*/
	//DOWN
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.5f, 0.0f); //orange
	glVertex3f(Matrixs::arrayAt(0, 0, c.f4.Data), Matrixs::arrayAt(0, 1, c.f4.Data), Matrixs::arrayAt(0, 2, c.f4.Data));
	glVertex3f(Matrixs::arrayAt(1, 0, c.f4.Data), Matrixs::arrayAt(1, 1, c.f4.Data), Matrixs::arrayAt(1, 2, c.f4.Data));
	glVertex3f(Matrixs::arrayAt(2, 0, c.f4.Data), Matrixs::arrayAt(2, 1, c.f4.Data), Matrixs::arrayAt(2, 2, c.f4.Data));
	glVertex3f(Matrixs::arrayAt(3, 0, c.f4.Data), Matrixs::arrayAt(3, 1, c.f4.Data), Matrixs::arrayAt(3, 2, c.f4.Data));
	glEnd();
	//draw wires
	glBegin(GL_LINES);
	glLineWidth((GLfloat)WIRE_THICK);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(Matrixs::arrayAt(0, 0, c.f4.Data) + WIRE_OFFSET, Matrixs::arrayAt(0, 1, c.f4.Data) + WIRE_OFFSET, Matrixs::arrayAt(0, 2, c.f4.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(1, 0, c.f4.Data) + WIRE_OFFSET, Matrixs::arrayAt(1, 1, c.f4.Data) + WIRE_OFFSET, Matrixs::arrayAt(1, 2, c.f4.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(2, 0, c.f4.Data) + WIRE_OFFSET, Matrixs::arrayAt(2, 1, c.f4.Data) + WIRE_OFFSET, Matrixs::arrayAt(2, 2, c.f4.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(3, 0, c.f4.Data) + WIRE_OFFSET, Matrixs::arrayAt(3, 1, c.f4.Data) + WIRE_OFFSET, Matrixs::arrayAt(3, 2, c.f4.Data) + WIRE_OFFSET);
	glEnd();
	
	//BACK (Technically facing me on init)
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 0.0f); //yellow
	glVertex3f(Matrixs::arrayAt(0, 0, c.f5.Data), Matrixs::arrayAt(0, 1, c.f5.Data), Matrixs::arrayAt(0, 2, c.f5.Data));
	glVertex3f(Matrixs::arrayAt(1, 0, c.f5.Data), Matrixs::arrayAt(1, 1, c.f5.Data), Matrixs::arrayAt(1, 2, c.f5.Data));
	glVertex3f(Matrixs::arrayAt(2, 0, c.f5.Data), Matrixs::arrayAt(2, 1, c.f5.Data), Matrixs::arrayAt(2, 2, c.f5.Data));
	glVertex3f(Matrixs::arrayAt(3, 0, c.f5.Data), Matrixs::arrayAt(3, 1, c.f5.Data), Matrixs::arrayAt(3, 2, c.f5.Data));
	glEnd();
	//draw wires
	glBegin(GL_LINES);
	glLineWidth((GLfloat)WIRE_THICK);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(Matrixs::arrayAt(0, 0, c.f5.Data) + WIRE_OFFSET, Matrixs::arrayAt(0, 1, c.f5.Data) + WIRE_OFFSET, Matrixs::arrayAt(0, 2, c.f5.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(1, 0, c.f5.Data) + WIRE_OFFSET, Matrixs::arrayAt(1, 1, c.f5.Data) + WIRE_OFFSET, Matrixs::arrayAt(1, 2, c.f5.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(2, 0, c.f5.Data) + WIRE_OFFSET, Matrixs::arrayAt(2, 1, c.f5.Data) + WIRE_OFFSET, Matrixs::arrayAt(2, 2, c.f5.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(3, 0, c.f5.Data) + WIRE_OFFSET, Matrixs::arrayAt(3, 1, c.f5.Data) + WIRE_OFFSET, Matrixs::arrayAt(3, 2, c.f5.Data) + WIRE_OFFSET);
	glEnd();
	/*
		Opp pair 3
	*/
	//FORTH
	glBegin(GL_QUADS);
	glColor3f(1.0f,1.0f,1.0f); //white
	glVertex3f(Matrixs::arrayAt(0, 0, c.f6.Data), Matrixs::arrayAt(0, 1, c.f6.Data), Matrixs::arrayAt(0, 2, c.f6.Data));
	glVertex3f(Matrixs::arrayAt(1, 0, c.f6.Data), Matrixs::arrayAt(1, 1, c.f6.Data), Matrixs::arrayAt(1, 2, c.f6.Data));
	glVertex3f(Matrixs::arrayAt(2, 0, c.f6.Data), Matrixs::arrayAt(2, 1, c.f6.Data), Matrixs::arrayAt(2, 2, c.f6.Data));
	glVertex3f(Matrixs::arrayAt(3, 0, c.f6.Data), Matrixs::arrayAt(3, 1, c.f6.Data), Matrixs::arrayAt(3, 2, c.f6.Data));
	glEnd();
	//draw wires
	glBegin(GL_LINES);
	glLineWidth((GLfloat)WIRE_THICK);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(Matrixs::arrayAt(0, 0, c.f6.Data) + WIRE_OFFSET, Matrixs::arrayAt(0, 1, c.f6.Data) + WIRE_OFFSET, Matrixs::arrayAt(0, 2, c.f6.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(1, 0, c.f6.Data) + WIRE_OFFSET, Matrixs::arrayAt(1, 1, c.f6.Data) + WIRE_OFFSET, Matrixs::arrayAt(1, 2, c.f6.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(2, 0, c.f6.Data) + WIRE_OFFSET, Matrixs::arrayAt(2, 1, c.f6.Data) + WIRE_OFFSET, Matrixs::arrayAt(2, 2, c.f6.Data) + WIRE_OFFSET);
	glVertex3f(Matrixs::arrayAt(3, 0, c.f6.Data) + WIRE_OFFSET, Matrixs::arrayAt(3, 1, c.f6.Data) + WIRE_OFFSET, Matrixs::arrayAt(3, 2, c.f6.Data) + WIRE_OFFSET);
	glEnd();
	//FPS_COUNT.Update();
	glFlush();
	return c;
}

Slice drawSlice() {
	Slice s;
	glTranslatef(1.0f, 1.0f, 0.0f); //up row
	s.c1 = drawBox();
	glTranslatef(-1.0f, 0.0f, 0.0f);
	s.c2 = drawBox();
	glTranslatef(-1.0f, 0.0f, 0.0f);
	s.c3 = drawBox();
	glTranslatef(0.0f, -1.0f, 0.0f); // middle row
	s.c4 = drawBox();
	glTranslatef(1.0f, 0.0f, 0.0f); //center?

	s.center = drawBox();

	//s.center.debug();
	glTranslatef(1.0f, 0.0f, 0.0f);
	s.c6 = drawBox();
	glTranslatef(0.0f, -1.0f, 0.0f); //bottom row
	s.c9 = drawBox();
	glTranslatef(-1.0f, 0.0f, 0.0f); 
	s.c8 = drawBox();
	glTranslatef(-1.0f, 0.0f, 0.0f); 
	s.c7 = drawBox();

	return s;
}

Rubik drawRubik() {
	Rubik r;
	glTranslatef( tra_x, tra_y, tra_z );
	r.s1 = drawSlice();
	glTranslatef(1.0f, 1.0f, 1.0f);
	drawSlice();
	glTranslatef(1.0f, 1.0f, -2.0f);
	drawSlice();
	return r;
}
int init(void) {
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
	return 1;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(
		0.0f, 0.0f, 9.0f, //"eyez"
		0.0f, 0.0f, 0.0f,
		0.0f, 9.0f, 0.0f); //perpective matrix (BOUNDS)
			//"upy"
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	drawRubik();
	/*drawSlice();*/
	/*drawBox();*/
	glFlush();
	glutSwapBuffers();
}

void resize(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(perspective_multiplier, resize_factor * w / h, resize_factor, 100 * resize_factor);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//optimized + opt Raster Render FIXME
void idle(int fps)
{
	if (!mouseDown)
	{
		xrot += 0.3f;
		yrot += 0.4f;
	}
	glutTimerFunc(fps, idle, 0);
	glutPostRedisplay();
}


void help_trigger(int key, int x, int y)
{
	cout << "U ------------> Rotate clockwise\n";
	cout << "Y ------------> Rotate counter clockwise\n";
	cout << "W ------------> Up\n";
	cout << "S ------------> Down\n";
	cout << "D ------------> Right\n";
	cout << "A ------------> Left\n";
	cout << "X ------------> Shrink -> Zoom Out\n";
	cout << "Z ------------> Grow -> Zoom In\n";
	cout << "F2 -----------> Show Position\n";
	cout << "F5 -----------> Refresh the render\n";
	cout << "Escape Key ---> Exit the program\n\n";
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case KEY_ESC:
		exit(1);
		break;
	case GLUT_KEY_F5:
		refresh();
		break;
	case 'w':
	case 'W':
		tra_x += 0.1f;
		break;
	case 's':
	case 'S':
		tra_x -= 0.1f;
		break;
	case 'a':
	case 'A':
		tra_z -= 0.1f;
		break;
	case 'd':
	case 'D':
		tra_z += 0.1f;
		break;
	case 'u':
	case 'U':
		xrot += 1.0f;
		yrot += 1.0f;
		xdiff += 1.0f;
		ydiff += 1.0f;
		break;
	case 'y':
	case 'Y':
		xrot -= 1.0f;
		yrot -= 1.0f;
		xdiff += 1.0f;
		ydiff += 1.0f;
		break;
	case 'h':
	case 'H':
		help_trigger(key, x, y);
		break;
	case 'Z':
		perspective_multiplier--;
		resize(500, 500);

		break;
	case 'X':
		perspective_multiplier++;
		resize(500, 500);
		break;

	}
	glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_F1)
	{
		fullscreen = !fullscreen;

		if (fullscreen) { glutFullScreen(); }
		else {
			glutReshapeWindow(640, 480); //working with standard aspect(s)
			glutPositionWindow(100, 100);
		}
	}
	/*if (key == GLUT_KEY_F2) { 
			cout << "Oblate Position : X[" << FPS_COUNT.GetFPS() << "]" << endl;
	}*/
	if (key == GLUT_KEY_F3) {
		//FIXME
	}
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = 1;
		xdiff = x - yrot;
		ydiff = -y + xrot;
	}
	else 
	{
		mouseDown = 0;
	}
}

void MOVE(int x, int y)
{


	////Debug
	//cout << "X-rot: " << xrot << "  Y-rot: " << yrot << endl <<
	//	"X-Diff: " << xdiff << "  Y-Diff: " << ydiff << endl <<
	//	"\tCurrent X & Y: " << '(' << x << "," << y << ')' << endl <<
	//	"Traj_x: " << tra_x << "  Traj_y: " << tra_y <<
	//	"  Traj_z: " << tra_z << endl;

	if (mouseDown) {
			xrot = y + ydiff;
			yrot = x - xdiff;
		/*else {
			xrot = -y - ydiff;
			yrot = - x + xdiff;
		}*/
	}

		//GLfloat change for if out of bounds
		//while for all as diff could be above abs(720)
		while (x >= 360) { x -= 360; }
		while (x <= -360) { x += 360; }

		//y
		while (y >= 360) { y -= 360; }
		while (y <= -360) { y += 360; }

		//xrot
		while (xrot >= 360) { xrot -= 360; }
		while (xrot <= -360) { xrot += 360; }

		//yrot
		while (yrot >= 360) { yrot -= 360; }
		while (yrot <= -360) { yrot += 360; }

		//xdiff
		while (xdiff >= 360) { xdiff -= 360; }
		while (xdiff <= -360) { xdiff += 360; } 

		//ydiff
		while (ydiff >= 360) { ydiff -= 360; }
		while (ydiff <= -360) { ydiff += 360; }
		glutPostRedisplay(); //update projection hoe
}



//MAIN
int main(int argc, char* argv[]) {
	//def of fps limiter
	//glutTimerFunc(RUN, UpdateTokens, current_time);
	
	//init cube
	//Cube C1
	//Rubiks(Cube C1
	glutInit(&argc, argv);

	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Rubik");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(MOVE);
	glutReshapeFunc(resize);
	//glutIdleFunc(idle(RUN);
	if (!init()) { return -1; }
	glutMainLoop();
	return 0;
}