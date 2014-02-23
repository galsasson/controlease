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
#include "InputNode.h"
#include "OutputNode.h"
#include "ConnectionResult.h"
#include "CanvasComponent.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class Constant : public CanvasComponent
{
public:
    Constant(Vec2f p, Vec2f s);
    ~Constant();
    
    void initInterface(Vec2f size);
    void update();
    void draw();
    void drawOutline();

    void mouseDown( cease::MouseEvent event );
	void mouseUp( cease::MouseEvent event);
	void mouseWheel( cease::MouseEvent event );
	void mouseMove( cease::MouseEvent event );
	void mouseDrag( cease::MouseEvent event );
    ConnectionResult* getConnectionStart( cease::MouseEvent event);
    ConnectionResult* getConnectionEnd( cease::MouseEvent event);
    vector<Node*> getInputNodes();
    vector<Node*> getOutputNodes();
    
    bool contains(Vec2f p);
    Vec2f getCanvasPos();
    
    float getValue(int i);
    void setValue(int i, float v);
    
private:
    void updateVal(float newVal);
    std::string getValueString();
    void applyBorders();
    
    bool immediateChange;
    
    Vec2f getLocalCoords(Vec2f p);
    Vec2f getCanvasCoords(Vec2f p);
    
    Rectf canvasRect;
    Rectf rect;
    Rectf titleRect;
    Rectf dragRect;
    Rectf valRect;
    
    // component dragging
    bool isCompDrag;
    Vec2f compDragAnchor;
    
    // value dragging
    float dragStartX;
    float dragX;
    bool isValDrag;
    float startVal;

    OutputNode *outputNode;
    InputNode *inputNode;
    
    float nextVal;
    float val;
    std::string valStr;
    float valInc;
    
    
    
    
};

#endif /* defined(__Controlease__Constant__) */
