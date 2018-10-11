//
//  Utilities.cpp
//  graphics
//
//  Created by Amr M. Kayid on 10/11/18.
//  Copyright © 2018 Amr M. Kayid. All rights reserved.
//

#include "Utilities.hpp"
#include <sys/timeb.h>                    // provides timeb and ftime

using namespace std;

//----------------------------------------------------------------------
//  checkGLerror - OpenGL error handler
//----------------------------------------------------------------------

void checkGLerror(const string& checkPoint) {
    GLenum errCode;
    while ((errCode = glGetError()) != GL_NO_ERROR) {
        cerr << "OpenGL Error at " << checkPoint << ": " << gluErrorString(errCode) << endl;
    }
    cerr.flush();                       // make sure that output appears
}

//----------------------------------------------------------------------
//  error - called if an error occurs
//----------------------------------------------------------------------

void error(const string& msg, bool fatal) {
    if (fatal) {
        cerr << "Error: " << msg << endl;
        exit(1);
    }
    else  cerr << "Warning: " << msg << endl;
}

//----------------------------------------------------------------------
//  Get millisecond time counts
//
//    Usage:
//        int start = getMilliCount();
//        ... (some computation)
//        int elapsed = getMillSpan(start);
//
//    Note that resolution is not to milliseconds, perhaps around 20msec.
//----------------------------------------------------------------------

int getMilliCount() {
    return glutGet(GLUT_ELAPSED_TIME);
}

int getMilliSpan(int nTimeStart) {
    return glutGet(GLUT_ELAPSED_TIME) - nTimeStart;
}
