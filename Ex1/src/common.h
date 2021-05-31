#pragma once

// switch between original and fixed output class
//#define VTKOUTPUT_H "vtkOutput.hpp"
#define VTKOUTPUT_H "vtkOutput_fixed.hpp"

// Argument positions for arg parser
#define ARG_XSIZE       1
#define ARG_YSIZE       2
#define ARG_SPACING     3
#define ARG_TYPE        4
#define ARG_BORDER_TYPES    9
#define ARG_FILE_NAME   10
#define ARG_COORD_X 11
#define ARG_COORD_Y 12


// Grid
#define BOTTOM  0
#define LEFT    1
#define TOP     2
#define RIGHT   3
#define CENTER  4
#define BOTTOM_LEFT 5
#define BOTTOM_RIGHT 6
#define TOP_RIGHT 7
#define TOP_LEFT 8
#define FORWARD 1
#define BACKWARD -1

#define COORD_X 0
#define COORD_Y 1
#define COORD_Z 2

#define BORDER_REFLECTIVE   0
#define BORDER_PERIODIC     1


// objects for SDF
#define TYPE_NOTYPE 0
#define TYPE_SPHERE 1
#define TYPE_RECT   2


// Sphere
#define ARG_SPHERE_CENTER_X 5
#define ARG_SPHERE_CENTER_Y 6
#define ARG_SPHERE_CENTER_Z 7
#define ARG_SPHERE_RADIUS   8



// Rectangle
#define ARG_RECT_MINCORNER_X    5
#define ARG_RECT_MINCORNER_Y    6
#define ARG_RECT_MAXCORNER_X    7
#define ARG_RECT_MAXCORNER_Y    8


// Misc
#define SUCCESS 0
#define EPSILON 1e-6