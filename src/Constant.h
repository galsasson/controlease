//
//  Constant.h
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#ifndef __Controlease__Constant__
#define __Controlease__Constant__

#include <iostream>
#include <sstream>
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rect.h"
#include <boost/container/vector.hpp>

#include "Controlease.h"
#include "ResourceManager.h"
#include "MouseListener.h"
#include "Node.h"
#include "CanvasComponent.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Constant : public CanvasComponent, public InputNode, public OutputNode
{
public:
    Constant(Vec2f p, Vec2f s);
    void initInterface(Vec2f size);
    void update();
    void draw();

    void connect(InputNode *node);

    void mouseDown( cease::MouseEvent event );
	void mouseUp( cease::MouseEvent event);
	void mouseWheel( cease::MouseEvent event );
	void mouseMove( cease::MouseEvent event );
	void mouseDrag( cease::MouseEvent event );
    
    bool contains(Vec2f p);
    
private:
    void updateVal(float newVal);
    std::string getValueString();
    
    Vec2f getLocalCoords(Vec2f p);
    
    Rectf canvasRect;
    Rectf rect;
    Rectf titleRect;
    Rectf dragRect;
    Rectf valRect;
    Vec2f inputNodePos;
    Vec2f outputNodePos;
    
    // component dragging
    bool isCompDrag;
    Vec2f compDragAnchor;
    
    // value dragging
    float dragStartX;
    float dragX;
    bool isValDrag;
    float startVal;
    
    
    
    InputNode *next;
    
    float val;
    std::string valStr;
    float valInc;
    
    
    
    
};

#endif /* defined(__Controlease__Constant__) */
