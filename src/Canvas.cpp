//
//  Canvas.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#include "Canvas.h"

Canvas::Canvas()
{
    
}

void Canvas::setup(Vec2f _pos, Vec2f _size, Vec2f _vSize)
{
    pos = _pos;
    size = _size;
    virtualSize = _vSize;
    topLeft = Vec2f(0, 0);
    scale = Vec2f(1, 1);
    
    fbo = gl::Fbo(virtualSize.x, virtualSize.y);
    
    isMouseDown = false;
    mouseHandler = this;
}

void Canvas::update()
{
    for (int i=0; i<programs.size(); i++)
    {
        programs[i]->update();
    }
    
    for (int i=0; i<components.size(); i++)
    {
        components[i]->update();
    }
}

void Canvas::draw()
{
    // draw on FBO
    fbo.bindFramebuffer();
    Area prevViewport = gl::getViewport();
    gl::setViewport(fbo.getBounds());
    gl::pushMatrices();
    gl::scale((Vec2f)getWindowSize() / (Vec2f)fbo.getSize() * scale);

    gl::clear(Color(1, 1, 1));
    gl::color(0.75, 0.75, 0.75);
    gl::lineWidth(1);

    for (float x=0; x<=virtualSize.x; x+=40)
    {
        gl::drawLine(Vec2f(x, 0), Vec2f(x, virtualSize.y));
    }
    for (float y=0; y<=virtualSize.y; y+=40)
    {
        gl::drawLine(Vec2f(0, y), Vec2f(virtualSize.x, y));
    }
    
    // the components on the canvas
    gl::color(0, 0, 0);
    for (int i=0; i<programs.size(); i++)
    {
        programs[i]->draw();
    }
    for (int i=0; i<components.size(); i++)
    {
        components[i]->draw();
    }
    
    gl::popMatrices();
    gl::setViewport(prevViewport);
    fbo.unbindFramebuffer();

    // draw the texture on screen
    gl::pushMatrices();
    gl::translate(pos);
    
    glEnable(GL_TEXTURE_2D);
    gl::color(Color(1, 1, 1));
    
    gl::draw(fbo.getTexture(), Area(Rectf(topLeft.x, (virtualSize.y - topLeft.y), (topLeft.x + size.x), (virtualSize.y - topLeft.y - size.y))), Rectf(0, size.y, size.x, 0));
    
    gl::lineWidth(2);
    gl::color(0, 0, 0);
    gl::drawStrokedRect(Rectf(0, 0, size.x, size.y));
    
    gl::popMatrices();
}

void Canvas::addProgram(Program *prog)
{
    programs.push_back(prog);
}

void Canvas::addComponent(CanvasComponent *comp)
{
    components.push_back(comp);
}

void Canvas::mouseDown(cease::MouseEvent event)
{
    isMouseDown = true;
    prevMouse = event.getPos();
}

void Canvas::mouseUp(cease::MouseEvent event)
{
    isMouseDown = false;
}

void Canvas::mouseWheel(cease::MouseEvent event)
{
    scale += event.getWheelIncrement() / 15;
    if (scale.x > 1) {
        scale = Vec2f(1, 1);
    }
    else if (scale.x < size.x / virtualSize.x) {
        scale = Vec2f(size / virtualSize);
    }
    
    checkBounds();
}

void Canvas::mouseMove(cease::MouseEvent event)
{
}

void Canvas::mouseDrag(cease::MouseEvent event)
{
    topLeft += prevMouse - event.getPos();
    prevMouse = event.getPos();
    
    checkBounds();
}

void Canvas::setMouseHandler(cease::MouseEvent event)
{
    // find the component the mouse is above (programs, wires, tools)
    for (int i=0; i<programs.size(); i++)
    {
        if (programs[i]->contains(event.getPos()))
        {
            mouseHandler = programs[i];
            return;
        }
    }
    for (int i=0; i<components.size(); i++)
    {
        if (components[i]->contains(event.getPos()))
        {
            mouseHandler = components[i];
            return;
        }
    }
    
    // if nothing else
    mouseHandler = this;
}

void Canvas::appMouseDown(MouseEvent event)
{
    cease::MouseEvent cevent(getLocalCoords(event.getPos()), event.getWheelIncrement());
    setMouseHandler(cevent);
    
    if (mouseHandler == this) {
        mouseHandler->mouseDown(cease::MouseEvent(event.getPos() - pos, event.getWheelIncrement()));
    }
    else {
        mouseHandler->mouseDown(cevent);
    }
}

void Canvas::appMouseUp(MouseEvent event)
{
    cease::MouseEvent cevent(getLocalCoords(event.getPos()), event.getWheelIncrement());
    mouseHandler->mouseUp(cevent);
}

void Canvas::appMouseWheel(MouseEvent event)
{
    cease::MouseEvent cevent(getLocalCoords(event.getPos()), event.getWheelIncrement());
//    setMouseHandler(cevent);
    mouseHandler->mouseWheel(cevent);
}

void Canvas::appMouseMove(MouseEvent event)
{
    cease::MouseEvent cevent(getLocalCoords(event.getPos()), event.getWheelIncrement());
    mouseHandler->mouseMove(cevent);
}

void Canvas::appMouseDrag(MouseEvent event)
{
    cease::MouseEvent cevent;
    if (mouseHandler == this) {
        cevent = cease::MouseEvent(event.getPos() - pos, event.getWheelIncrement());
    }
    else {
        cevent = cease::MouseEvent(getLocalCoords(event.getPos()), event.getWheelIncrement());
    }
    
    mouseHandler->mouseDrag(cevent);
}

bool Canvas::contains(Vec2f p)
{
    return Area(pos, pos+size).contains(p);
}

void Canvas::checkBounds()
{
    if (topLeft.x < 0) {
        topLeft.x = 0;
    }
    else if (topLeft.x > virtualSize.x*scale.x - size.x) {
        topLeft.x = virtualSize.x*scale.x - size.x;
    }
    if (topLeft.y < 0) {
        topLeft.y = 0;
    }
    else if (topLeft.y > virtualSize.y*scale.y - size.y) {
        topLeft.y = virtualSize.y*scale.y - size.y;
    }
}

Vec2f Canvas::getLocalCoords(Vec2f worldCoords)
{
    return (worldCoords - pos + topLeft) / scale;
}
