//
//  Canvas.cpp
//  tunnelvars
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
}

void Canvas::update()
{
    for (int i=0; i<programs.size(); i++)
    {
        programs[0]->update();
    }
}

void Canvas::draw()
{
    // draw on FBO
    fbo.bindFramebuffer();
    Area prevViewport = gl::getViewport();
    gl::setViewport(fbo.getBounds());
    gl::enableAlphaBlending();
    gl::pushMatrices();
    gl::scale((Vec2f)getWindowSize() / (Vec2f)fbo.getSize() * scale);

    gl::clear(Color(1, 1, 1));
    gl::color(0.8, 0.8, 0.8);
    gl::lineWidth(2);

    for (float x=0; x<=virtualSize.x; x+=40)
    {
        gl::drawLine(Vec2f(x, 0), Vec2f(x, virtualSize.y));
    }
    for (float y=0; y<=virtualSize.y; y+=40)
    {
        gl::drawLine(Vec2f(0, y), Vec2f(virtualSize.x, y));
    }
    
    gl::color(0, 0, 0);
    for (int i=0; i<programs.size(); i++)
    {
        programs[0]->draw();
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
    
    gl::popMatrices();
}

void Canvas::addProgram(Program *prog)
{
    programs.push_back(prog);
}

void Canvas::mouseDown(MouseEvent event)
{
    isMouseDown = true;
    prevMouse = event.getPos();
}

void Canvas::mouseUp(MouseEvent event)
{
    isMouseDown = false;
}

void Canvas::mouseWheel(MouseEvent event)
{
    scale += event.getWheelIncrement() / 15;
    if (scale.x > 1) {
        scale = Vec2f(1, 1);
    }
    else if (scale.x < size.x / virtualSize.x) {
        scale = Vec2f(size / virtualSize);
    }
    
}

void Canvas::mouseMove(MouseEvent event)
{
    
}

void Canvas::mouseDrag(MouseEvent event)
{
    topLeft += prevMouse - event.getPos();
    prevMouse = event.getPos();
    
    checkBounds();
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


