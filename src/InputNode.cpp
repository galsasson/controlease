//
//  InputNode.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/22/14.
//
//

#include "InputNode.h"
#include "CanvasComponent.h"

InputNode::InputNode(CanvasComponent *comp) : Node(comp)
{
    setName("Input");
}

InputNode::~InputNode()
{
    
}

void InputNode::draw()
{
    gl::color(ResourceManager::getInstance().getColor(2));
    
    if (bDrawActive) {
        gl::drawSolidCircle(pos, 5);
        bDrawActive = false;
    }
    else {
        gl::color(0, 0, 0, 0.5);
        gl::drawSolidCircle(pos, 3);
        gl::color(0, 0, 0);
        gl::drawStrokedCircle(pos, 3);
    }
    
    if (bDisplayName) {
        gl::color(0, 0, 0);
        ResourceManager::getInstance().getTextureFont()->drawString(name, pos + Vec2f(5, 3));
    }
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


