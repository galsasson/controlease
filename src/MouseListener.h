//
//  MouseListener.h
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#ifndef Controlease_MouseListener_h
#define Controlease_MouseListener_h

#include "cinder/app/MouseEvent.h"

#include "Controlease.h"
#include "ConnectionResult.h"

using namespace ci::app;
using namespace ci;

namespace cease {
    
    class MouseEvent
    {
    public:
        MouseEvent() { pos = Vec2f(0, 0); wheelInc = 0; }
        MouseEvent(Vec2f p, float w) { pos = p; wheelInc = w; }
        Vec2f getPos() { return pos; }
        float getWheelIncrement() { return wheelInc; }
        Vec2f pos;
        float wheelInc;
    };
}

class MouseListener
{
public:
    virtual ~MouseListener() {}
	virtual void mouseDown( cease::MouseEvent event ) = 0;
	virtual void mouseUp( cease::MouseEvent event) = 0;
	virtual void mouseWheel( cease::MouseEvent event ) = 0;
	virtual void mouseMove( cease::MouseEvent event ) = 0;
	virtual void mouseDrag( cease::MouseEvent event ) = 0;
    virtual bool contains(Vec2f p) = 0;
};

#endif
