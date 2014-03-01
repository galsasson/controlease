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

void Canvas::setup(Vec2f _pos, Vec2f _size)
{
    pos = _pos;
    size = _size;
    virtualSize = Vec2f(CANVAS_WIDTH, CANVAS_HEIGHT);
    topLeft = Vec2f(0, 0);
    scale = Vec2f(1, 1);
    
    fbo = gl::Fbo(virtualSize.x, virtualSize.y);
    
    isMouseDown = false;
    currentWire = NULL;
    focusComponent = NULL;
    dragComponent = NULL;
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

void Canvas::addComponent(Tool *tool)
{
    if (tool == NULL) {
        return;
    }
    
    switch (tool->type) {
        case TOOL_TYPE_PROGRAM:
            addComponent(new Program(topLeft + Vec2f(30, 30)));
            break;
        case TOOL_TYPE_NUMBER:
            addComponent(new Number(topLeft + Vec2f(30, 30), Vec2f(100, 40)));
            break;
        case TOOL_TYPE_SPLIT:
            addComponent(new Split(topLeft + Vec2f(30, 30), Vec2f(100, 40)));
            break;
        case TOOL_TYPE_ADD:
            addComponent(new Add(topLeft + Vec2f(30, 30), Vec2f(100, 40)));
            break;
        case TOOL_TYPE_SUB:
            addComponent(new Sub(topLeft + Vec2f(30, 30), Vec2f(100, 40)));
            break;
        case TOOL_TYPE_MUL:
            addComponent(new Mult(topLeft + Vec2f(30, 30), Vec2f(100, 40)));
            break;
        case TOOL_TYPE_DIV:
            addComponent(new Div(topLeft + Vec2f(30, 30), Vec2f(100, 40)));
            break;
        case TOOL_TYPE_RANDOM:
            addComponent(new Random(topLeft + Vec2f(30, 30), Vec2f(100, 40)));
            break;
        case TOOL_TYPE_SINE:
            addComponent(new Sine(topLeft + Vec2f(30, 30), Vec2f(100, 40)));
            break;
            
        default:
            break;
    }
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

bool Canvas::contains(Vec2f p)
{
    return Area(pos, pos+size).contains(p);
}

ConnectionResult* Canvas::getConnection(cease::MouseEvent event)
{
    return NULL;
}

void Canvas::keyDown(cinder::app::KeyEvent event)
{
    if (event.getCode() == 8)   // DEL
    {
        if (focusComponent) {
            deleteComponent(focusComponent);
        }
    }
}

void Canvas::keyUp(cinder::app::KeyEvent event)
{
}

CanvasComponent* Canvas::getMouseComponent(Vec2f p)
{
    for (int i=0; i<components.size(); i++)
    {
        if (components[i]->contains(p))
        {
            return components[i];
        }
    }
    
    return NULL;
}

void Canvas::appMouseDown(MouseEvent event)
{
    cease::MouseEvent cevent(getLocalCoords(event.getPos()), event.getWheelIncrement());
    dragComponent = NULL;
    
    if (event.isControlDown()) {
        cevent.pos = event.getPos() - pos;
        return mouseDown(cevent);
    }
    
    focusComponent = getMouseComponent(cevent.getPos());
    if (focusComponent == NULL) {
        return;
    }

    if (focusComponent->isDragPoint(cevent)) {
        dragComponent = focusComponent;
    }

    // check for hot spot in component (draggable elements)
    if (focusComponent->isHotspot(cevent))
    {
        return focusComponent->mouseDown(cevent);
    }
    
    // check for connection node click
    ConnectionResult *con = focusComponent->getConnectionStart(cevent);
    handleConnectionStart(con);
}

void Canvas::appMouseUp(MouseEvent event)
{
    cease::MouseEvent cevent(getLocalCoords(event.getPos()), event.getWheelIncrement());
    CanvasComponent *comp = getMouseComponent(cevent.getPos());
    dragComponent = NULL;
    
    // we are dragging a wire
    if (currentWire != NULL) {
        if (comp == NULL) {
            return handleConnectionEnd(NULL);
        }

        return handleConnectionEnd(comp->getConnectionEnd(cevent));
    }
    
    if (focusComponent == NULL) {
        return mouseUp(cevent);
    }
    
    focusComponent->mouseUp(cevent);
}

void Canvas::appMouseWheel(MouseEvent event)
{
    cease::MouseEvent cevent(getLocalCoords(event.getPos()), event.getWheelIncrement());
//    setMouseHandler(cevent);
//    mouseHandler->mouseWheel(cevent);
}

void Canvas::appMouseMove(MouseEvent event)
{
    cease::MouseEvent cevent(getLocalCoords(event.getPos()), event.getWheelIncrement());
//    mouseHandler->mouseMove(cevent);
}

void Canvas::appMouseDrag(MouseEvent event)
{
    cease::MouseEvent cevent(getLocalCoords(event.getPos()), event.getWheelIncrement());
    
    if (event.isControlDown()) {
        cevent.pos = event.getPos() - pos;
        return this->mouseDrag(cevent);
    }
    
    if (currentWire != NULL) {
        return currentWire->setEnd(cevent.getPos());
    }
    
    if (dragComponent) {
        dragComponent->mouseDrag(cevent);
    }
}


void Canvas::appKeyDown(cinder::app::KeyEvent event)
{
    if (focusComponent)
    {
        KeyboardListener *keyListener = focusComponent->getCurrentKeyboardListener();
        if (keyListener) {
            return keyListener->keyDown(event);
        }
    }
    
    keyDown(event);
}

void Canvas::appKeyUp(cinder::app::KeyEvent event)
{
    keyUp(event);
}

void Canvas::setSize(Vec2f newSize)
{
    size = newSize;
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
    if (con == NULL) {
        return;
    }
    
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

void Canvas::deleteComponent(CanvasComponent *comp)
{
    if (comp == NULL) {
        return;
    }
    
    // first, delete all wires
    vector<Node*> inodes = comp->getInputNodes();
    for (int i=0; i<inodes.size(); i++)
    {
        Wire *w = popWireWithNode(inodes[i]);
        if (w) {
            delete w;
        }
    }
    vector<Node*> onodes = comp->getOutputNodes();
    for (int i=0; i<onodes.size(); i++)
    {
        Wire *w = popWireWithNode(onodes[i]);
        if (w) {
            delete w;
        }
    }

    // if component is also the mouse handler, set it to this
    if (comp == focusComponent) {
        focusComponent = NULL;
    }
    
    if (comp == dragComponent) {
        dragComponent = NULL;
    }
    
    // now delete the component
    for (int i=0; i<components.size(); i++)
    {
        if (components[i] == comp)
        {
            components.erase(components.begin() + i);
            delete comp;
            comp = NULL;
        }
    }
}

