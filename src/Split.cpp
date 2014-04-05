//
//  Split.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "Split.h"

Split::Split(Canvas *c, Vec2f p) : CanvasComponent(c, ComponentType::COMPONENT_TYPE_SPLIT)
{
    Vec2f s(40, 50);
    canvasRect = Rectf(p, p+s);
    initInterface(s);
    
    nextVal = 0;
    updateVal(0);
    
    immediateChange = false;
}

Split::~Split()
{
}

void Split::initInterface(Vec2f size)
{
    rect = Rectf(Vec2f(0, 0), size);
    titleRect = Rectf(2, 2, size.x-20, 20);
    plusRect = Rectf(size.x-12, 5, size.x-2, 15);
    inputNodes.push_back(new InputNode(0, this, Vec2f(6, 26)));
    for (int i=0; i<2; i++)
    {
        outputNodes.push_back(new OutputNode(i, this, Vec2f(size.x - 6, titleRect.y2 + 6 + i*9)));
    }
    nextOutputPos = Vec2f(size.x - 6, titleRect.y2 + 24);
}

void Split::update()
{
    if (!immediateChange)
    {
        if (nextVal != val) {
            updateVal(nextVal);
        }
    }
}

void Split::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(rect, 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(rect, 2);
    
    // draw title
    ResourceManager::getInstance().getTextureFont()->drawString("Split", titleRect);
    gl::drawLine(Vec2f(0, titleRect.y2), Vec2f(rect.getWidth(), titleRect.y2));
    
    // draw + sign
    gl::drawLine(Vec2f(plusRect.x1, plusRect.y1 + plusRect.getHeight()/2), Vec2f(plusRect.x2, plusRect.y1 + plusRect.getHeight()/2));
    gl::drawLine(Vec2f(plusRect.x1 + plusRect.getWidth()/2, plusRect.y1), Vec2f(plusRect.x1 + plusRect.getWidth()/2, plusRect.y2));
    
    // draw nodes
    inputNodes[0]->draw();
    for (int i=0; i<outputNodes.size(); i++)
    {
        outputNodes[i]->draw();
    }
    
    gl::popMatrices();
}

void Split::translate(Vec2f offset)
{
    canvasRect += offset;
}

Rectf Split::getBounds()
{
    return canvasRect;
}

void Split::mouseDown(cease::MouseEvent event)
{
    Vec2f local = toLocal(event.getPos());
    if (plusRect.contains(local)) {
        // add another output
        outputNodes.push_back(new OutputNode(outputNodes.size(), this, nextOutputPos));
        nextOutputPos.y += 9;
        canvasRect.y2 += 9;
        rect.y2 += 9;
    }
    
    compDragAnchor = event.getPos();
}

void Split::mouseDrag(cease::MouseEvent event)
{
    canvasRect += event.getPos() - compDragAnchor;
    compDragAnchor = event.getPos();
    applyBorders();
}

void Split::mouseUp( cease::MouseEvent event)
{
}

void Split::mouseWheel( cease::MouseEvent event ) {}
void Split::mouseMove( cease::MouseEvent event ) {}

bool Split::isDragPoint(cease::MouseEvent event)
{
    Vec2f local = toLocal(event.getPos());
    
    return titleRect.contains(local);
}

bool Split::isHotspot(cease::MouseEvent event)
{
    Vec2f local = toLocal(event.getPos());
    
    return titleRect.contains(local) || plusRect.contains(local);
}

float Split::getValue(int i)
{
    return val;
}

void Split::setValue(int i, float v)
{
    if (immediateChange) {
        updateVal(v);
    }
    else {
        nextVal = v;
    }
}

void Split::updateVal(float newVal)
{
    val = newVal;

    for (int i=0; i<outputNodes.size(); i++)
    {
        outputNodes[i]->updateVal(val);
    }
}
