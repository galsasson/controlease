//
//  Number.h
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#ifndef __Controlease__Number__
#define __Controlease__Number__

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

class Number : public CanvasComponent
{
public:
    Number(Canvas *c);
    ~Number();
    
    void initNew(Vec2f pos);
    void initFromXml(const XmlTree& xml, bool createNodes = true);
    XmlTree getXml();
    
    void update();
    void draw();

    void mouseDown(const cease::MouseEvent& event);
	void mouseUp(const cease::MouseEvent& event);
	void mouseDrag(const cease::MouseEvent& event);
    bool isDragPoint(const cease::MouseEvent& event);
    bool isHotspot(const cease::MouseEvent& event);
    
    float getValue(int i);
    void setValue(int i, float v);
    void outputConnected(int i);
    void outputDisconnected(int i);
        
private:
    void updateVal(float newVal);
    
    bool immediateChange;
    
    Rectf valRect;
    
    // component dragging
    bool isCompDrag;
    Vec2f compDragAnchor;
    
    // value dragging
    float dragLastX;
    float dragX;
    bool isValDrag;

    float nextVal;
    float val;
    std::string valStr;
    float valInc;
};

#endif /* defined(__Controlease__Number__) */
