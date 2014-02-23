//
//  Slider.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "Slider.h"

Slider::Slider(Vec2f p, Vec2f s)
{
    pos = p;
    size = s;
    
    progbarRect = Rectf(2, size.y/2, size.x-2, size.y-2);
    
    val = 0;
    min = -100;
    max = 100;
    
    graphicVal = Vec2f(size.x/2, size.y/2);
}

void Slider::update()
{
    
}

void Slider::draw()
{
    gl::pushMatrices();
    gl::translate(pos);
    
    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(Rectf(Vec2f(0, 0), size), 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(Rectf(Vec2f(0, 0), size), 2);
    
    // draw progress line
    gl::color(0, 0, 0);
    gl::drawSolidRect(progbarRect);
    
    gl::popMatrices();
}

void Slider::mouseDown(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    if (progbarRect.contains(local))
    {
//        val = lmap(x, pos.x, pos.x+size.x, min, max);
//        graphicVal.x = x;
    }
        
}

void Slider::mouseDrag(cease::MouseEvent event)
{
    float x = getLocalCoords(event.getPos()).x;
    val = lmap(x, pos.x, pos.x+size.x, min, max);
    graphicVal.x = x;
}
void Slider::mouseUp( cease::MouseEvent event) {}
void Slider::mouseWheel( cease::MouseEvent event ) {}
void Slider::mouseMove( cease::MouseEvent event ) {}

bool Slider::contains(Vec2f p)
{
    return Area(pos, pos+size).contains(p);
}

Vec2f Slider::getLocalCoords(Vec2f p)
{
    return p-pos;
}
