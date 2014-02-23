//
//  Slider.h
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#ifndef __Controlease__Slider__
#define __Controlease__Slider__

#include <iostream>
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include <boost/container/vector.hpp>

#include "MouseListener.h"
#include "Node.h"
#include "CanvasComponent.h"

using namespace ci;
using namespace ci::app;

class Slider : public CanvasComponent
{
public:
    Slider(Vec2f p, Vec2f s);
    void update();
    void draw();
    
    void mouseDown( cease::MouseEvent event );
	void mouseUp( cease::MouseEvent event);
	void mouseWheel( cease::MouseEvent event );
	void mouseMove( cease::MouseEvent event );
	void mouseDrag( cease::MouseEvent event );
    
    bool contains(Vec2f p);

    
private:
    Vec2f getLocalCoords(Vec2f p);
    
    Vec2f pos;
    Vec2f size;
    Rectf progbarRect;
    float progbarWidth;
    Vec2f graphicVal;
    
    float val;
    float min, max;
    
    
};


#endif /* defined(__Controlease__Slider__) */
