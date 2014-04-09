//
//  Number.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "Number.h"

Number::Number(Canvas *c) : CanvasComponent(c)
{
    type = ComponentType::COMPONENT_TYPE_NUMBER;
    
    immediateChange = false;
    valInc = 1;
    dragLastX = dragX = 0;
    isValDrag = false;
    isCompDrag = false;
}

Number::~Number()
{
}

void Number::initNew(Vec2f pos)
{
    CanvasComponent::initNew(pos);
    
    setSize(Vec2f(100, 40));
    setName("Number");
    
    addNewInputNode();
    addNewOutputNode();

    nextVal = 0;
    updateVal(0);
}

void Number::initFromXml(const XmlTree& xml)
{
    CanvasComponent::initFromXml(xml);
    
    nextVal = roundFloat(xml.getAttributeValue<float>("value"));
    updateVal(nextVal);
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
    gl::drawSolidRoundedRect(localRect, 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(localRect, 2);
    
    if (isValDrag) {
        gl::color(0.95, 0.95, 0.5);
        gl::drawSolidRect(valRect);
    }
    
    // draw title
    gl::color(0, 0, 0);
    ResourceManager::getInstance().getTextureFont()->drawString(name, titleRect);
    gl::drawLine(Vec2f(0, titleRect.y2), Vec2f(localRect.getWidth(), titleRect.y2));
    
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
        dragLastX = local.x;
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
    
    if (isValDrag) {
        dragX = local.x;
        if (immediateChange) {
            updateVal(nextVal + (dragX - dragLastX) * valInc);
        }
        else {
            float inc = (event.keyModifiers&MouseEvent::SHIFT_DOWN)?valInc*0.01:valInc;
            nextVal += (dragX - dragLastX) * inc;
            nextVal = roundFloat(nextVal);
            dragLastX = dragX;
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
        dragLastX = 0;
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

XmlTree Number::getXml()
{
    XmlTree xml = CanvasComponent::getXml();
    
    xml.setAttribute("value", roundFloat(val));
    
    return xml;
}

void Number::updateVal(float newVal)
{
    // round to 4 decimal places
    val = newVal;
    
    valStr = getValueString(val);
    Vec2f valStrSize = ResourceManager::getInstance().getTextureFont()->measureString(valStr);
    valRect = Rectf(localRect.getWidth()/2 - valStrSize.x/2, localRect.getHeight()/2+3, localRect.getWidth()/2 + valStrSize.x/2, localRect.getHeight()-2);
    
    outputNodes[0]->updateVal(val);
}
