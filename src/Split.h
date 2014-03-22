//
//  Split.h
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#ifndef __Controlease__Split__
#define __Controlease__Split__

#include <iostream>
#include <sstream>
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rect.h"
#include <boost/container/vector.hpp>

#include "Controlease.h"
#include "ResourceManager.h"
#include "MouseListener.h"
#include "InputNode.h"
#include "OutputNode.h"
#include "ConnectionResult.h"
#include "CanvasComponent.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class Split : public CanvasComponent
{
public:
    Split(Vec2f p, Vec2f s);
    ~Split();
    
    void initInterface(Vec2f size);
    void update();
    void draw();
    void drawOutline();

    void translate(Vec2f offset);
    Rectf getBounds();
    
    void mouseDown( cease::MouseEvent event );
	void mouseUp( cease::MouseEvent event);
	void mouseWheel( cease::MouseEvent event );
	void mouseMove( cease::MouseEvent event );
	void mouseDrag( cease::MouseEvent event );
    bool isDragPoint( cease::MouseEvent event);
    bool isHotspot( cease::MouseEvent event);
    
    float getValue(int i);
    void setValue(int i, float v);
    
private:
    void updateVal(float newVal);
    void applyBorders();
    
    bool immediateChange;
    
    Rectf rect;
    Rectf titleRect;
    Rectf plusRect;
    Rectf dragRect;
    Rectf valRect;
    
    // component dragging
    Vec2f compDragAnchor;
    
    Vec2f nextOutputPos;
    
    float nextVal;
    float val;
    std::string valStr;
    float valInc;
    
    
    
    
};

#endif /* defined(__Controlease__Split__) */
