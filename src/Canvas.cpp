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
    // each node in the canvas has a unique ID
    Node::globalNodeID = 0;
}

void Canvas::setup(Vec2f _pos, Vec2f _size)
{
    pos = _pos;
    size = _size;
    virtualSize = Vec2f(CANVAS_WIDTH, CANVAS_HEIGHT);
    topLeft = Vec2f(CANVAS_WIDTH/2, CANVAS_HEIGHT/2);
    scale = Vec2f(1, 1);
    
    gl::Fbo::Format msaaFormat;
    msaaFormat.setSamples(8);
    fbo = gl::Fbo(virtualSize.x, virtualSize.y, msaaFormat);
    gridFbo = gl::Fbo(virtualSize.x, virtualSize.y);
    drawGridFbo();
    
    currentWire = NULL;
    focusComponent = NULL;
    dragComponent = NULL;
    infoBox = NULL;
    bMagnifyingGlass = false;
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
    
    if (infoBox != NULL) {
        infoBox->update();
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

    // draw grid fbo
    gl::color(1, 1, 1);
    gl::draw(gridFbo.getTexture());

    // the components on the canvas
    gl::color(0, 0, 0);
    for (int i=0; i<components.size(); i++)
    {
        if (components[i] != focusComponent)
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
    
    if (infoBox != NULL) {
        infoBox->draw();
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
    if (comp == NULL) {
        return;
    }
    
    components.push_back(comp);
}

void Canvas::mouseDown(cease::MouseEvent event)
{
    prevMouse = event.getPos();
}

void Canvas::mouseUp(cease::MouseEvent event)
{
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

void Canvas::keyDown(cinder::app::KeyEvent event)
{
    console() << "keyDown: "<<event.getCode()<<endl;
    
    if (event.getCode() == 8)   // DEL
    {
        if (focusComponent) {
            deleteComponent(focusComponent);
        }
    }
    else if (event.getCode() == 122)    // 'z'
    {
        bMagnifyingGlass = true;
    }
}

void Canvas::keyUp(cinder::app::KeyEvent event)
{
    if (event.getCode() == 122) {
        bMagnifyingGlass = false;
    }
}

void Canvas::appMouseDown(MouseEvent event)
{
    cease::MouseEvent cevent(getLocalCoords(event.getPos()), event.getWheelIncrement(), event.getNativeModifiers());
    dragComponent = NULL;
    
    if (event.isControlDown()) {
        cevent.pos = event.getPos() - pos;
        return mouseDown(cevent);
    }
    
    focusComponent = getComponentUnder(cevent.getPos());
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
    handleConnectionStart(focusComponent->getConnectionStart(cevent));
}

void Canvas::appMouseUp(MouseEvent event)
{
    cease::MouseEvent cevent(getLocalCoords(event.getPos()), event.getWheelIncrement(), event.getNativeModifiers());
    CanvasComponent *comp = getComponentUnder(cevent.getPos());
    dragComponent = NULL;
    
    // we are dragging a wire
    if (currentWire != NULL) {
        if (comp == NULL) {
            // mouse is NOT above a component, return no connection
            return handleConnectionEnd(ConnectionResult());
        }

        // mouse IS above a component
        return handleConnectionEnd(comp->getConnectionEnd(cevent));
    }
    
    if (focusComponent == NULL) {
        return mouseUp(cevent);
    }
    
    focusComponent->mouseUp(cevent);
}

void Canvas::appMouseWheel(MouseEvent event)
{
    cease::MouseEvent cevent(getLocalCoords(event.getPos()), event.getWheelIncrement(), event.getNativeModifiers());
//    setMouseHandler(cevent);
//    mouseHandler->mouseWheel(cevent);
}

void Canvas::appMouseMove(MouseEvent event)
{
//    if (!bMagnifyingGlass) {
//        return;
//    }
    
    cease::MouseEvent cevent(getLocalCoords(event.getPos()), event.getWheelIncrement(), event.getNativeModifiers());

    // look for node underneath cursor to display NodeInfoBox (tooltip)
    for (int i=0; i<components.size(); i++)
    {
        Node *n = components[i]->getNodeBelow(cevent);
        if (n == NULL) {
            continue;
        }
        
        if (infoBox != NULL) {
            if (infoBox->node == n) {
                // we are currently showing this node info box
                return;
            }
            
            // this is a different info box
            delete infoBox;
            infoBox = NULL;
        }
        
        infoBox = new NodeInfoBox(n);
        return;
    }
    
    if (infoBox != NULL) {
        delete infoBox;
        infoBox = NULL;
    }

//    mouseHandler->mouseMove(cevent);
}

void Canvas::appMouseDrag(MouseEvent event)
{
    if (event.isShiftDown()) {
        console() << "drag shift down!"<<endl;
    }
    cease::MouseEvent cevent(getLocalCoords(event.getPos()), event.getWheelIncrement(), event.getNativeModifiers());
    
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


vector<InputNode*> Canvas::getInputNodesAtArea(Vec2f center, float rad)
{
    vector<InputNode*> nodes;
    
    for (int i=0; i<components.size(); i++)
    {
        vector<Node*> compNodes = components[i]->getInputNodes();
        for (int n=0; n<compNodes.size(); n++)
        {
            Node* node = compNodes[n];
            if ((node->getCanvasPos() - center).length() < rad)
            {
                nodes.push_back((InputNode*)node);
            }
        }
    }
    
    return nodes;
}

void Canvas::makeConnection(int outputID, int inputID)
{
    // find output node
    Node *onode = getNodeWithID(outputID);
    Node *inode = getNodeWithID(inputID);

    if (onode != NULL && inode != NULL)
    {
        makeConnection((OutputNode*)onode, (InputNode*)inode);
    }
}

void Canvas::makeConnection(OutputNode *onode, int inputID)
{
    if (onode->isConnected()) {
        return;
    }
    
    Node* inode = getNodeWithID(inputID);
    if (inode == NULL) {
        return;
    }
    
    makeConnection(onode, (InputNode*)inode);
}

void Canvas::makeConnection(OutputNode *onode, InputNode *inode)
{
    if (onode->isConnected() || inode->isConnected()) {
        return;
    }
    
    Wire *wire = new Wire();
    wire->addConnectable(ConnectionResult(TYPE_OUTPUT, onode));
    wire->addConnectable(ConnectionResult(TYPE_INPUT, inode));
    wires.push_back(wire);
}

void Canvas::disconnectNode(Node *node)
{
    Wire *w = popWireWithNode(node);
    if (w) {
        delete w;
    }
}

void Canvas::reset()
{
    Node::globalNodeID = 0;
    
    for (int i=components.size()-1; i>=0; i--)
    {
        deleteComponent(components[i]);
    }
}

XmlTree Canvas::getXml()
{
    XmlTree canvasTree("Canvas", "");
    
    // add components xmls
    XmlTree canvasComps("Components", "");
    for (int i=0; i<components.size(); i++)
    {
        canvasComps.push_back(components[i]->getXml());
    }
    canvasTree.push_back(canvasComps);
    
    // add wires xmls
    XmlTree wiresXml("Wires", "");
    for (int i=0; i<wires.size(); i++)
    {
        wiresXml.push_back(wires[i]->getXml());
    }
    canvasTree.push_back(wiresXml);
    
    return canvasTree;
}

void Canvas::initFromXml(const XmlTree& xml)
{
    // first, clear the canvas
    reset();
    
    // create components
    XmlTree componentsNode = xml.getChild("Components");
    for(XmlTree::ConstIter iter = componentsNode.begin(); iter != componentsNode.end(); ++iter)
    {
        if (iter->getTag() != "CanvasComponent")
        {
            console() << "error: unexpected tag: " << iter->getTag() << endl;
            continue;
        }

        XmlTree compXml = iter->getChild("");
        CanvasComponent *newComp = ComponentFactory::newComponent(this, compXml);
        if (newComp != NULL) {
            components.push_back(newComp);
        }
    }
    
    // connect wires
    XmlTree wiresNode = xml.getChild("Wires");
    for(XmlTree::ConstIter iter = wiresNode.begin(); iter != wiresNode.end(); ++iter)
    {
        if (iter->getTag() != "Wire") {
            console() << "error: unexpected tag: " << iter->getTag() << endl;
            continue;
        }
        
        XmlTree wireXml = iter->getChild("");
        makeConnection(wireXml.getAttributeValue<int>("outputID"), wireXml.getAttributeValue<int>("inputID"));
    }
}



void Canvas::drawGridFbo()
{
    gridFbo.bindFramebuffer();
    Area prevViewport = gl::getViewport();
    gl::setViewport(gridFbo.getBounds());

    gl::clear(Color(0.0, 0.0, 0.0));
    gl::color(0.35, 0.35, 0.35);
    gl::lineWidth(1);
    
    for (float x=0; x<=virtualSize.x; x+=40)
    {
        gl::drawLine(Vec2f(x, 0), Vec2f(x, virtualSize.y));
    }
    for (float y=0; y<=virtualSize.y; y+=40)
    {
        gl::drawLine(Vec2f(0, y), Vec2f(virtualSize.x, y));
    }

    gl::setViewport(prevViewport);
    gridFbo.unbindFramebuffer();
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

void Canvas::handleConnectionStart(ConnectionResult con)
{
    switch (con.type)
    {
        case TYPE_NONE:
            return;
        case TYPE_DISCONNECT_INPUT:
            // find the wire that was clicked
            currentWire = popWireWithNode(con.node);
            if (currentWire != NULL) {
                currentWire->disconnectInput();
            }
            break;
        case TYPE_DISCONNECT_OUTPUT:
            currentWire = popWireWithNode(con.node);
            if (currentWire != NULL) {
                currentWire->disconnectOutput();
            }
            break;
        case TYPE_OUTPUT:
            currentWire = new Wire();
            currentWire->addConnectable(con);
            break;
        default:
            return;
    }
}

void Canvas::handleConnectionEnd(ConnectionResult con)
{
    if (con.type == TYPE_NONE) {
        delete currentWire;
        currentWire = NULL;
        return;
    }

    if (currentWire->addConnectable(con)) {
        wires.push_back(currentWire);
    }
    else {
        // no connection was made, delete current wire
        delete currentWire;
    }
    
    // reset current wire to NULL (added to 'wires' or deleted at this point)
    currentWire = NULL;
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

    // if component is also the focus or drag component, set to NULL
    if (comp == focusComponent) {
        focusComponent = NULL;
    }
    
    if (comp == dragComponent) {
        dragComponent = NULL;
    }
    
    // remove nodeinfobox
    if (infoBox != NULL) {
        delete infoBox;
        infoBox = NULL;
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

CanvasComponent* Canvas::getComponentUnder(Vec2f p)
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

Node* Canvas::getNodeWithID(int id)
{
    for (int i=0; i<components.size(); i++)
    {
        Node* node = components[i]->getNodeWithID(id);
        if (node != NULL) {
            return node;
        }
    }
    
    return NULL;
}

