//
//  Oscillator.h
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#ifndef __Controlease__Oscillator__
#define __Controlease__Oscillator__

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

class Oscillator : public CanvasComponent
{
public:
    Oscillator(Canvas *c);
    ~Oscillator();
    
    void initNew(Vec2f pos);
    void initFromXml(XmlTree xml);
    
    void update();
    void draw();

    void mouseDown(const cease::MouseEvent& event);
	void mouseDrag(const cease::MouseEvent& event);
    bool isDragPoint(const cease::MouseEvent& event);
    bool isHotspot(const cease::MouseEvent& event);
    
    float getValue(int i);
    void setValue(int i, float v);
    
private:
    void updateVal(float newVal);
    std::string getValueString();
    
    bool immediateChange;
    
    Rectf valRect;
    
    // component dragging
    Vec2f compDragAnchor;
    
    long lastUpdate;
    double time;
    float frequency;
    float val;
    std::string valStr;
    
    
    
    
};

#endif /* defined(__Controlease__Oscillator__) */
