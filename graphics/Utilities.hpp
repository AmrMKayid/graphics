//
//  Utilities.hpp
//  graphics
//
//  Created by Amr M. Kayid on 10/11/18.
//  Copyright © 2018 Amr M. Kayid. All rights reserved.
//

#ifndef Utilities_hpp
#define Utilities_hpp


#define _USE_MATH_DEFINES                // makes M_PI available in VC++

#include <cmath>                        // standard math definitions
#include <string>                        // C++ strings
#include <iostream>                        // C++ I/O
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

const GLdouble GEOM_EPS = 1.0E-6;        // tiny number for floating compare
const GLdouble GEOM_INF = 1.0E+30;        // a huge number

#ifndef M_PI                            // define M_PI if not defined
#define M_PI 3.14159265358979323846
#endif

// ---------------------------------------------------------------
// Utility functions for random number generation
// ---------------------------------------------------------------
inline double unifRand(double low, double high)
{ return low + (high-low)*double(rand())/double(RAND_MAX); }

inline bool unifBool()
{ return unifRand(0, 1) < 0.5; }    // this is true 50% of the time

// ---------------------------------------------------------------
// Clamps val to a value between min and max
// ---------------------------------------------------------------
inline double clamp(double val, double min, double max)
{
    if (max < GEOM_INF && val > max) val = max;
    else if(val < min) val = min;
    
    return val;
}

// ---------------------------------------------------------------
// Convertion to/from radians and degrees
// ---------------------------------------------------------------
inline double deg2rad(double deg)
{ return deg * (M_PI/180.0); }

inline double rad2deg(double rad)
{ return rad * 180.0/M_PI; }

//----------------------------------------------------------------------
//  checkGLerror - OpenGL error handler
//      OpenGL does not generate error messages or exceptions.  In order
//      to check for errors, insert this call at strategic points within
//      the program.  The checkPoint argument is a label for where the
//      error checking was invoked from. Example:
//
//              checkGLerror("After texture-map setup")
//----------------------------------------------------------------------
void checkGLerror(const std::string& checkPoint);

//----------------------------------------------------------------------
//  error - error handler
//----------------------------------------------------------------------
void error(const std::string& msg, bool fatal);

//----------------------------------------------------------------------
//  get millisecond time counts
//----------------------------------------------------------------------
int getMilliCount();
int getMilliSpan(int nTimeStart);


#endif /* Utilities_hpp */
