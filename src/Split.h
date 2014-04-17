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
    Split(Canvas *c);
    ~Split();
    
    void initNew(Vec2f pos);
    void initFromXml(const XmlTree& xml, bool createNodes = true);
    XmlTree getXml();
        
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
    
    bool immediateChange;
    
    // component dragging
    Vec2f compDragAnchor;
    
    float nextVal;
    float val;
};

#endif /* defined(__Controlease__Split__) */
