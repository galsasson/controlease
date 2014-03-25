//
//  OutputNode.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/22/14.
//
//

#include "OutputNode.h"
#include "CanvasComponent.h"

OutputNode::OutputNode(int i, CanvasComponent *comp, Vec2f p)
{
    index = i;
    component = comp;
    pos = p;
    
    bDrawActive = false;
    
    next = NULL;
    prev = NULL;
    lastVal = 0;
    setName("Output");
    bDisplayName = false;
}

OutputNode::~OutputNode()
{
    
}

void OutputNode::draw()
{
    float rad = 3;
    if (bDrawActive) {
        rad = 4;
    }
    gl::color(ResourceManager::getInstance().getColor(4));
    gl::drawSolidCircle(pos, rad);

    if (!bDrawActive) {
        gl::color(0, 0, 0, 0.5);
        gl::drawSolidCircle(pos, 3);
    }
    else {
        bDrawActive = false;
    }
    
    gl::color(0, 0, 0);
    gl::drawStrokedCircle(pos, 3);
    
//    if (bDisplayName) {
//    }
}

void OutputNode::updateVal(float val, bool force)
{
    if (lastVal == val) {
        if (!force) {
            return;
        }
    }
    
    bDrawActive = true;
    lastVal = val;
    if (next != NULL) {
        next->updateVal(val);
    }
}

void OutputNode::connect(Node *node)
{
    next = node;
    
    // update the new input node with the component value
    lastVal = component->getValue(index);
    updateVal(lastVal, true);
    
    // update the component about the connection
    component->outputConnected(index);
}

void OutputNode::disconnect(Node *node)
{
    if (next == node) {
        next = NULL;
        component->outputDisconnected(index);
    }
}

bool OutputNode::isConnected()
{
    return (next != NULL);
}

std::string OutputNode::getEndpointName()
{
    if (next == NULL) {
        return std::string("Input");
    }
    
    return std::string(next->name);
}

Vec2f OutputNode::getCanvasPos()
{
    return component->getCanvasPos() + pos;
}

