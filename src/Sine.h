//
//  Sine.h
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#ifndef __Controlease__Sine__
#define __Controlease__Sine__

#include <iostream>
#include <sstream>
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rect.h"
#include <boost/container/vector.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

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

class Sine : public CanvasComponent
{
public:
    Sine(Vec2f p, Vec2f s);
    ~Sine();
    
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
    bool isDragPoint(cease::MouseEvent event);
    bool isHotspot( cease::MouseEvent event);
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
    Vec2f compDragAnchor;
    
    OutputNode *outputNode;
    InputNode *inputNode;
    
    long lastUpdate;
    double time;
    float frequency;
    float val;
    std::string valStr;
    
    
    
    
};

#endif /* defined(__Controlease__Sine__) */
