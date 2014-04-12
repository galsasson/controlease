//
//  NodeInfoBox.cpp
//  Controlease
//
//  Created by Gal Sasson on 3/22/14.
//
//

#include "NodeInfoBox.h"

NodeInfoBox::NodeInfoBox(Node *n)
{
    setNode(n);
}

void NodeInfoBox::setNode(Node *n)
{
    node = n;
    float nameWidth = ResourceManager::getInstance().getTextureFont()->measureString(n->getName()).length();
    Vec2f pos = n->getCanvasPos() + Vec2f(-nameWidth - 20, 0);
    canvasRect = Rectf(0, 0, 0, 0);
    canvasRect.offset(pos);
    setSize(nameWidth, 40);
}

void NodeInfoBox::update()
{
    strVal = getValueString(node->getLastVal());
    float valWidth = ResourceManager::getInstance().getTextureFont()->measureString(strVal).length();
    if (valWidth > localRect.getWidth()) {
        setSize(valWidth, 40);
    }
}

void NodeInfoBox::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());
    
    gl::color(0.95, 0.95, 0.5);
    gl::drawSolidRoundedRect(localRect, 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(localRect, 2);
    
    // draw title
    ResourceManager::getInstance().getTextureFont()->drawString(node->getName(), nameRect);
    
    ResourceManager::getInstance().getTextureFont()->drawString(strVal, valueRect);
    
    gl::popMatrices();
    
    glPushAttrib(GL_ENABLE_BIT);
    glLineStipple(1, 0xf0f0);
    gl::enable(GL_LINE_STIPPLE);
    
    gl::color(0.75, 0.75, 0.75);
    gl::drawLine(canvasRect.getUpperRight(), node->getCanvasPos());
    
    glPopAttrib();

}

void NodeInfoBox::setSize(float w, float h)
{
    localRect = Rectf(0, 0, w, h);
    canvasRect.x2 = canvasRect.x1 + w;
    canvasRect.y2 = canvasRect.y1 + h;
    nameRect = Rectf(2, 2, w, h/2);
    valueRect = Rectf(2, h/2, w, h);
}