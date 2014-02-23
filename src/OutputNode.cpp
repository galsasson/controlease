//
//  OutputNode.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/22/14.
//
//

#include "OutputNode.h"

OutputNode::OutputNode(int i, CanvasComponent *comp, Vec2f p)
{
    index = i;
    component = comp;
    pos = p;
    
    next = NULL;
    prev = NULL;
}

void OutputNode::draw()
{
    gl::color(1, 1, 1);
    gl::drawSolidCircle(pos, 3);
    gl::color(0, 0, 0);
    gl::drawStrokedCircle(pos, 3);
}

bool OutputNode::contains(Vec2f p)
{
    return (p-pos).length() <= 3;
}

void OutputNode::updateVal(float val)
{
    if (next != NULL) {
        next->updateVal(val);
    }
}

void OutputNode::connect(Node *node)
{
    next = node;
    updateVal(component->getValue(index));
}

void OutputNode::disconnect(Node *node)
{
    if (next == node) {
        next = NULL;
    }
}

bool OutputNode::isConnected()
{
    return (next != NULL);
}

Vec2f OutputNode::getCanvasPos()
{
    return component->getCanvasPos() + pos;
}

