//
//  OutputNode.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/22/14.
//
//

#include "OutputNode.h"
#include "CanvasComponent.h"

OutputNode::OutputNode(CanvasComponent *comp) : Node(comp)
{
    setName("Output");    
}

OutputNode::~OutputNode()
{
    
}

void OutputNode::draw()
{
    gl::color(ResourceManager::getInstance().getColor(4));
    
    if (bDrawActive) {
        gl::drawSolidCircle(pos, 5);
        bDrawActive = false;
    }
    else {
        gl::drawSolidCircle(pos, 3);
        gl::color(0, 0, 0, 0.5);
        gl::drawSolidCircle(pos, 3);
        gl::color(0, 0, 0);
        gl::drawStrokedCircle(pos, 3);
    }
    
    if (bDisplayName) {
        ResourceManager::getInstance().getTextureFont()->drawString(name, pos - Vec2f(nameSize.x+5, -3));
    }
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

