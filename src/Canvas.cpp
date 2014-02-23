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
    currentWire = NULL;
    focusComponent = NULL;
}

void Canvas::update()
{
    for (int i=0; i<components.size(); i++)
    {
        components[i]->update();
    }
    for (int i=0; i<wires.size(); i++)
    {
        wires[i]->update();
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
    for (int i=0; i<components.size(); i++)
    {
        components[i]->draw();
    }

    // draw focused component
    if (focusComponent != NULL) {
        focusComponent->draw();
        focusComponent->drawOutline();
    }

    // draw wires
    for (int i=0; i<wires.size(); i++)
    {
        wires[i]->draw();
    }
    if (currentWire != NULL) {
        currentWire->draw();
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

ConnectionResult* Canvas::getConnection(cease::MouseEvent event)
{
    return NULL;
}


void Canvas::setMouseHandler(cease::MouseEvent event)
{
    // find the component the mouse is above (programs, wires, tools)
    for (int i=0; i<components.size(); i++)
    {
        if (components[i]->contains(event.getPos()))
        {
            mouseHandler = (MouseListener*)components[i];
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
        focusComponent = NULL;
        mouseHandler->mouseDown(cease::MouseEvent(event.getPos() - pos, event.getWheelIncrement()));
    }
    else {
        ConnectionResult* con = ((CanvasComponent*)mouseHandler)->getConnectionStart(cevent);
        if (con != NULL && con->type != TYPE_INPUT) {
            // node was clicked handle it
            handleConnectionStart(con);
        }
        else {
            // no node was clicked, do the normal gui stuff
            focusComponent = (CanvasComponent*)mouseHandler;
            mouseHandler->mouseDown(cevent);
        }
    }
}

void Canvas::appMouseUp(MouseEvent event)
{
    cease::MouseEvent cevent(getLocalCoords(event.getPos()), event.getWheelIncrement());
    setMouseHandler(cevent);
    
    if (currentWire != NULL)
    {
        // special case if user release on canvas
        if (mouseHandler == this) {
            handleConnectionEnd(NULL);
        }
        else {
            handleConnectionEnd(((CanvasComponent*)mouseHandler)->getConnectionEnd(cevent));
        }
    }
    else {
        mouseHandler->mouseUp(cevent);
    }
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
    
    
    if (currentWire != NULL) {
        currentWire->setEnd(cevent.getPos());
    }
    else {
        mouseHandler->mouseDrag(cevent);
    }
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

void Canvas::handleConnectionStart(ConnectionResult *con)
{
    // currentWire will always be NULL is we got here
    
    // check if we grabbed an already connected wire
    if (con->type == TYPE_DISCONNECT_INPUT) {
        // find the wire that was clicked
        currentWire = popWireWithNode(con->node);
        if (currentWire != NULL) {
            currentWire->disconnectInput();
        }
        return;
    }
    
    if (con->type == TYPE_DISCONNECT_OUTPUT) {
        currentWire = popWireWithNode(con->node);
        if (currentWire != NULL) {
            currentWire->disconnectOutput();
        }
        return;
    }
    
    if (con->type == TYPE_OUTPUT) {
        currentWire = new Wire();
        currentWire->addConnectable(con);
    }
    
    // delete the connection result
    delete con;
}

void Canvas::handleConnectionEnd(ConnectionResult *con)
{
    if (con == NULL) {
        // no connection
        delete currentWire;
        currentWire = NULL;
        return;
    }

    if (currentWire->addConnectable(con)) {
        wires.push_back(currentWire);
    }
    else {
        delete currentWire;
    }
    currentWire = NULL;
    
    // delete the connection result
    delete con;
}

Wire* Canvas::popWireWithNode(Node *node)
{
    for (int i=0; i<wires.size(); i++)
    {
        if (wires[i]->haveNode(node)) {
            Wire* wire = wires[i];
            wires.erase(wires.begin() + i);
            return wire;
        }
    }
    
    return NULL;
}
