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
        MouseEvent() { pos = Vec2f(0, 0); wheelInc = 0; keyModifiers = 0; }
        MouseEvent(Vec2f p, float w) { pos = p; wheelInc = w; keyModifiers = 0; }
        MouseEvent(Vec2f p, float w, uint32_t modifiers) { pos = p; wheelInc = w; keyModifiers = modifiers; }
        Vec2f getPos() const { return pos; }
        float getWheelIncrement() const { return wheelInc; }
        Vec2f pos;
        float wheelInc;
        uint32_t keyModifiers;
    };
}

class MouseListener
{
public:
    virtual ~MouseListener() {}
	virtual void mouseDown(const cease::MouseEvent& event) {};
	virtual void mouseUp(const cease::MouseEvent& event) {};
	virtual void mouseWheel(const cease::MouseEvent& event) {};
	virtual void mouseMove(const cease::MouseEvent& event) {};
	virtual void mouseDrag(const cease::MouseEvent& event) {};
    virtual bool contains(const Vec2f& p) { return false; };
};

#endif
