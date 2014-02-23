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

#include "Controlease.h"
#include "MouseListener.h"
#include "ResourceManager.h"
#include "Program.h"
#include "ConnectionResult.h"
#include "CanvasComponent.h"
#include "Node.h"
#include "Wire.h"

using namespace ci;
using namespace ci::app;

class Canvas : public MouseListener
{
public:
    Canvas();
    void setup(Vec2f _pos, Vec2f _size, Vec2f _vSize);
    void update();
    void draw();
    void addComponent(CanvasComponent *comp);

	void mouseDown( cease::MouseEvent event );
	void mouseUp( cease::MouseEvent event);
	void mouseWheel( cease::MouseEvent event );
	void mouseMove( cease::MouseEvent event );
	void mouseDrag( cease::MouseEvent event );
    bool contains(Vec2f p);
    ConnectionResult* getConnection(cease::MouseEvent event);
    
    
    void appMouseDown( MouseEvent event );
	void appMouseUp( MouseEvent event);
	void appMouseWheel( MouseEvent event );
	void appMouseMove( MouseEvent event );
	void appMouseDrag( MouseEvent event );
    
    void appKeyDown( KeyEvent event );
    void appKeyUp( KeyEvent event );


    Vec2f pos;
    Vec2f size;
    Vec2f virtualSize;
    Vec2f topLeft;
    Vec2f scale;

private:
    void setMouseHandler(cease::MouseEvent event);
    void checkBounds();
    Vec2f getLocalCoords(Vec2f worldCoords);
    void handleConnectionStart(ConnectionResult *con);
    void handleConnectionEnd(ConnectionResult *con);
    Wire* popWireWithNode(Node *node);
    void deleteFocusComponent();
    
    boost::container::vector<CanvasComponent*> components;
    
    
    // hold the canvas
    gl::Fbo fbo;
    
    // interaction
    CanvasComponent *focusComponent;
    MouseListener *mouseHandler;
    Vec2f prevMouse;
    bool isMouseDown;
    
    // connecting graph
    boost::container::vector<Wire*> wires;
    Wire* currentWire;
};


#endif /* defined(__tunnelvars__Canvas__) */
