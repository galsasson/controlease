//
//  Canvas.h
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#ifndef __Controlease__Canvas__
#define __Controlease__Canvas__

#include <iostream>
#include <boost/container/vector.hpp>
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/Vbo.h"

#include "MouseListener.h"
#include "ResourceManager.h"
#include "Program.h"
#include "CanvasComponent.h"
#include "Node.h"

using namespace ci;
using namespace ci::app;

class Canvas : public MouseListener
{
public:
    Canvas();
    void setup(Vec2f _pos, Vec2f _size, Vec2f _vSize);
    void update();
    void draw();
    void addProgram(Program *program);
    void addComponent(CanvasComponent *comp);

	void mouseDown( cease::MouseEvent event );
	void mouseUp( cease::MouseEvent event);
	void mouseWheel( cease::MouseEvent event );
	void mouseMove( cease::MouseEvent event );
	void mouseDrag( cease::MouseEvent event );
    
    void appMouseDown( MouseEvent event );
	void appMouseUp( MouseEvent event);
	void appMouseWheel( MouseEvent event );
	void appMouseMove( MouseEvent event );
	void appMouseDrag( MouseEvent event );
    
    bool contains(Vec2f p);

    Vec2f pos;
    Vec2f size;
    Vec2f virtualSize;
    Vec2f topLeft;
    Vec2f scale;

private:
    void setMouseHandler(cease::MouseEvent event);
    void checkBounds();
    Vec2f getLocalCoords(Vec2f worldCoords);
    
    boost::container::vector<Program*> programs;
    boost::container::vector<CanvasComponent*> components;
    
    
    // hold the canvas
    gl::Fbo fbo;
    
    // used to draw on screen
    TriMesh triMesh;
    
    // interaction
    MouseListener *mouseHandler;
    Vec2f prevMouse;
    bool isMouseDown;
};


#endif /* defined(__tunnelvars__Canvas__) */
