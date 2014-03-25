//
//  InputNode.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/22/14.
//
//

#include "InputNode.h"
#include "CanvasComponent.h"

InputNode::InputNode(int i, CanvasComponent *comp, Vec2f p)
{
    index = i;
    component = comp;
    pos = p;
    
    next = NULL;
    prev = NULL;
    lastVal = 0;
    setName("Input");
    
    bDrawActive = false;
}

InputNode::~InputNode()
{
    
}

void InputNode::draw()
{
    float rad = 3;
    if (bDrawActive) {
        rad = 4;
    }
    gl::color(ResourceManager::getInstance().getColor(2));
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
}

void InputNode::updateVal(float val, bool force)
{
    bDrawActive = true;
    lastVal = val;
    component->setValue(index, val);
}

void InputNode::connect(Node *node)
{
    prev = node;
    
    component->inputConnected(index);
}

void InputNode::disconnect(Node *node)
{
    if (prev == node) {
        prev = NULL;
        lastVal = 0;
        component->setValue(index, 0);
        component->inputDisconnected(index);
    }
    
}

bool InputNode::isConnected()
{
    return (prev != NULL);
}

Vec2f InputNode::getCanvasPos()
{
    return component->getCanvasPos() + pos;
}