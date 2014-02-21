//
//  Canvas.h
//  tunnelvars
//
//  Created by Gal Sasson on 2/19/14.
//
//

#ifndef __tunnelvars__Canvas__
#define __tunnelvars__Canvas__

#include <iostream>
#include <boost/container/vector.hpp>
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/Vbo.h"

#include "ResourceManager.h"
#include "Program.h"

using namespace ci;
using namespace ci::app;

class Canvas
{
public:
    Canvas();
    void setup(Vec2f _pos, Vec2f _size, Vec2f _vSize);
    void update();
    void draw();
    void addProgram(Program *program);
    
	void mouseDown( MouseEvent event );
	void mouseUp( MouseEvent event);
	void mouseWheel( MouseEvent event );
	void mouseMove( MouseEvent event );
	void mouseDrag( MouseEvent event );
    
private:
    void checkBounds();
    
    boost::container::vector<Program*> programs;
    
    Vec2f pos;
    Vec2f size;
    Vec2f virtualSize;
    Vec2f topLeft;
    Vec2f scale;
    
    // hold the canvas
    gl::Fbo fbo;
    
    // used to draw on screen
    TriMesh triMesh;
    
    // interaction
    Vec2i prevMouse;
    bool isMouseDown;
};


#endif /* defined(__tunnelvars__Canvas__) */
