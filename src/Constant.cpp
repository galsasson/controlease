//
//  Constant.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "Constant.h"

Constant::Constant(Vec2f p, Vec2f s)
{
    canvasRect = Rectf(p, p+s);
    initInterface(s);
    
    next = NULL;
    startVal = 0;
    updateVal(0);
    valInc = 1;
}

void Constant::initInterface(Vec2f size)
{
    rect = Rectf(Vec2f(0, 0), size);
    Vec2f titleSize = ResourceManager::getInstance().getTextureFont()->measureString("Constant");
    titleRect = Rectf(size.x/2 - titleSize.x/2, 2, size.x/2 + titleSize.x/2, size.y/2-2);
    inputNodePos = Vec2f(6, size.y*3/4);
    outputNodePos = Vec2f(size.x - 6, size.y*3/4);
    
    dragStartX = dragX = 0;
    isValDrag = false;
    isCompDrag = false;
}

void Constant::update()
{
    
}

void Constant::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(rect, 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(rect, 2);
    
    // draw title
    ResourceManager::getInstance().getTextureFont()->drawString("Constant", titleRect);
    gl::drawLine(Vec2f(0, rect.getHeight()/2), Vec2f(rect.getWidth(), rect.getHeight()/2));
    
    // draw nodes
    gl::drawStrokedCircle(inputNodePos, 3);
    gl::drawStrokedCircle(outputNodePos, 3);
    
    ResourceManager::getInstance().getTextureFont()->drawString(valStr, valRect);
    gl::popMatrices();
}

void Constant::connect(InputNode *node)
{
    next = node;
}

void Constant::mouseDown(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    if (valRect.contains(local)) {
        isValDrag = true;
        dragStartX = local.x;
        startVal = val;
    }
    else {
        isCompDrag = true;
        compDragAnchor = event.getPos();
    }
    
    
    
//    if (progbarRect.contains(local))
//    {
        //        val = lmap(x, pos.x, pos.x+size.x, min, max);
        //        graphicVal.x = x;
//    }
    
}

void Constant::mouseDrag(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    if (isValDrag) {
        dragX = local.x;
        updateVal(startVal + (dragX - dragStartX) * valInc);
    }

    if (isCompDrag) {
        canvasRect += event.getPos() - compDragAnchor;
//        pos += local - compDragAnchor;
        compDragAnchor = event.getPos();
    }
}

void Constant::mouseUp( cease::MouseEvent event)
{
    if (isValDrag) {
        isValDrag = false;
        dragX = 0;
        dragStartX = 0;
    }
    
    if (isCompDrag) {
        isCompDrag = false;
    }
}

void Constant::mouseWheel( cease::MouseEvent event ) {}
void Constant::mouseMove( cease::MouseEvent event ) {}

bool Constant::contains(Vec2f p)
{
    return canvasRect.contains(p);
}

Vec2f Constant::getLocalCoords(Vec2f p)
{
    return p-canvasRect.getUpperLeft();
}

void Constant::updateVal(float newVal)
{
    val = newVal;
    valStr = getValueString();
    console() << valStr << endl;
    Vec2f valStrSize = ResourceManager::getInstance().getTextureFont()->measureString(valStr);
    valRect = Rectf(rect.getWidth()/2 - valStrSize.x/2, rect.getHeight()/2+3,
                    rect.getWidth()/2 + valStrSize.x/2, rect.getHeight()-2);
    
    // pass on to next node
    if (next != NULL) {
        next->updateVal(val);
    }
}

std::string Constant::getValueString()
{
    char buff[128];
    sprintf(buff, "%." FLOAT_PRECISION "f", val);
    std::ostringstream str;
    str << buff;
    return str.str();
}
