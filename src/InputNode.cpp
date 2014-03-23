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
    name = "Input";
    
    bFillEllipse = true;
}

InputNode::~InputNode()
{
    
}

void InputNode::draw()
{
    gl::color(ResourceManager::getInstance().getColor(2));
    gl::drawSolidCircle(pos, 3);
    
    if (bFillEllipse) {
        gl::color(0, 0, 0, 0.5);
        gl::drawSolidCircle(pos, 3);
    }
    else {
        bFillEllipse = true;
    }
    
    gl::color(0, 0, 0);
    gl::drawStrokedCircle(pos, 3);
}

bool InputNode::contains(Vec2f p)
{
    return (p-pos).length() <= 5;
}

void InputNode::updateVal(float val)
{
    bFillEllipse = false;
    lastVal = val;
    component->setValue(index, val);
}

void InputNode::connect(Node *node)
{
    prev = node;
}

void InputNode::disconnect(Node *node)
{
    if (prev == node) {
        prev = NULL;
    }
    
    lastVal = 0;
    component->setValue(index, 0);
}

bool InputNode::isConnected()
{
    return (prev != NULL);
}

Vec2f InputNode::getCanvasPos()
{
    return component->getCanvasPos() + pos;
}