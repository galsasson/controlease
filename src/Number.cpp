//
//  Number.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "Number.h"

Number::Number(Canvas *c, Vec2f p, Vec2f s) : CanvasComponent(c, ComponentType::COMPONENT_TYPE_NUMBER)
{
    canvasRect = Rectf(p, p+s);
    initInterface(s);
    
    startVal = 0;
    nextVal = 0;
    updateVal(0);
    valInc = 0.1;
    
    immediateChange = false;
}

Number::~Number()
{
}

void Number::initInterface(Vec2f size)
{
    rect = Rectf(Vec2f(0, 0), size);
    titleRect = Rectf(2, 2, size.x-2, size.y/2-2);
    inputNodes.push_back(new InputNode(0, this, Vec2f(6, size.y*3/4)));
    outputNodes.push_back(new OutputNode(0, this, Vec2f(size.x - 6, size.y*3/4)));
    
    dragStartX = dragX = 0;
    isValDrag = false;
    isCompDrag = false;
    
    setName("Number");
}

void Number::update()
{
    if (!immediateChange)
    {
        if (nextVal != val) {
            updateVal(nextVal);
        }
    }
}

void Number::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(rect, 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(rect, 2);
    
    if (isValDrag) {
        gl::color(0.95, 0.95, 0.5);
        gl::drawSolidRect(valRect);
    }
    
    // draw title
    gl::color(0, 0, 0);
    ResourceManager::getInstance().getTextureFont()->drawString(name, titleRect);
    gl::drawLine(Vec2f(0, rect.getHeight()/2), Vec2f(rect.getWidth(), rect.getHeight()/2));
    
    // draw nodes
    inputNodes[0]->draw();
    outputNodes[0]->draw();
    
    gl::color(0, 0, 0);
    ResourceManager::getInstance().getTextureFont()->drawString(valStr, valRect);
    gl::popMatrices();
}

void Number::mouseDown(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    
    if (valRect.contains(local)) {
        isValDrag = true;
        dragStartX = local.x;
        startVal = val;
    }
    else {
        isCompDrag = true;
        isValDrag = false;
        compDragAnchor = event.getPos();
    }
}

void Number::mouseDrag(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    
    console() << "mouse drag "<<event.getPos().x<<endl;
    if (event.keyModifiers&MouseEvent::SHIFT_DOWN)
    {
        console() << "number mouse drag shift down"<<endl;
    }
    
    if (isValDrag) {
        dragX = local.x;
        if (immediateChange) {
            updateVal(startVal + (dragX - dragStartX) * valInc);
        }
        else {
            float inc = (event.keyModifiers&MouseEvent::SHIFT_DOWN)?valInc*50:valInc;
            nextVal = startVal + (dragX - dragStartX) * inc;
        }
    }

    if (isCompDrag) {
        canvasRect += event.getPos() - compDragAnchor;
        compDragAnchor = event.getPos();
        applyBorders();
    }
}

void Number::mouseUp(const cease::MouseEvent& event)
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

bool Number::isDragPoint(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    
    return valRect.contains(local) || titleRect.contains(local);
}

bool Number::isHotspot(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    
    return valRect.contains(local) || titleRect.contains(local);
}

float Number::getValue(int i)
{
    return val;
}

void Number::setValue(int i, float v)
{
    // this is a hack: don't update disconnect 0 in Number components
    if (!inputNodes[i]->isConnected()) {
        return;
    }
    
    if (immediateChange) {
        updateVal(v);
    }
    else {
        nextVal = v;
    }
}

void Number::outputConnected(int i)
{
    setName(outputNodes[i]->getEndpointName());
}

void Number::outputDisconnected(int i)
{
    setName("Number");
}

void Number::updateVal(float newVal)
{
    val = newVal;
    valStr = getValueString(val);
    Vec2f valStrSize = ResourceManager::getInstance().getTextureFont()->measureString(valStr);
    valRect = Rectf(rect.getWidth()/2 - valStrSize.x/2, rect.getHeight()/2+3,
                    rect.getWidth()/2 + valStrSize.x/2, rect.getHeight()-2);
    
    outputNodes[0]->updateVal(val);
}
