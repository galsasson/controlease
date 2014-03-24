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
    
    bFillEllipse = true;
    
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
    gl::color(ResourceManager::getInstance().getColor(4));
    gl::drawSolidCircle(pos, 3);

    if (bFillEllipse) {
        gl::color(0, 0, 0, 0.5);
        gl::drawSolidCircle(pos, 3);
//        bFillEllipse = false;
    }
    else {
        bFillEllipse = true;
    }
    
    gl::color(0, 0, 0);
    gl::drawStrokedCircle(pos, 3);
    
//    if (bDisplayName) {
//    }
}

bool OutputNode::contains(Vec2f p)
{
    console() << "checking if "<<p<<" is close to "<<pos<<endl;
    return (p-pos).length() <= 5;
}

void OutputNode::updateVal(float val)
{
    if (lastVal == val) {
        return;
    }
    
    bFillEllipse = false;
    lastVal = val;
    if (next != NULL) {
        next->updateVal(val);
    }
}

void OutputNode::connect(Node *node)
{
    next = node;
    lastVal = component->getValue(index);
    updateVal(lastVal);
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

