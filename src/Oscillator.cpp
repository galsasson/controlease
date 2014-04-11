//
//  Oscillator.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "Oscillator.h"

#define TWO_PI_DIV_1000 M_PI/500

Oscillator::Oscillator(Canvas *c) : CanvasComponent(c)
{
    setType(ComponentType::COMPONENT_TYPE_OSCILLATOR);
    immediateChange = false;
}

Oscillator::~Oscillator()
{
}

void Oscillator::initNew(Vec2f pos)
{
    CanvasComponent::initNew(pos);
    setSize(Vec2f(100, 40));
    
    addNewInputNode();
    inputNodes[0]->setName("Frequency");
    addNewOutputNode();
    
    frequency = 0;
    updateVal(0);
    time = 0;
}

void Oscillator::initFromXml(const XmlTree& xml)
{
    CanvasComponent::initFromXml(xml);
    
    val = xml.getAttributeValue<float>("value");
    time = xml.getAttributeValue<double>("time");
}

XmlTree Oscillator::getXml()
{
    XmlTree xml = CanvasComponent::getXml();
    
    xml.setAttribute("value", val);
    xml.setAttribute("time", time);
    
    return xml;
}


void Oscillator::update()
{
    if (frequency == 0 || !inputNodes[0]->isConnected()) {
        return;
    }
    
    timeval timeVal;
    gettimeofday(&timeVal, NULL);
    long millis = (timeVal.tv_sec * 1000) + (timeVal.tv_usec / 1000);
    
    long frameTime = millis - lastUpdate;
    
    time += M_PI * 2 * frameTime * frequency / 1000;
    if (time > M_PI*2) {
        time -= M_PI*2;
    }
    else if (time < -M_PI*2)
    {
        time += M_PI*2;
    }
    
    updateVal(sin(time)/2 + 0.5);
    
    lastUpdate = millis;
}

void Oscillator::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(localRect, 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(localRect, 2);
    
    // draw title
    ResourceManager::getInstance().getTextureFont()->drawString(name, titleRect);
    gl::drawLine(Vec2f(0, titleRect.getHeight()), Vec2f(localRect.getWidth(), titleRect.getHeight()));
    
    // draw nodes
    inputNodes[0]->draw();
    outputNodes[0]->draw();
    
    gl::color(0, 0, 0);
    ResourceManager::getInstance().getTextureFont()->drawString(valStr, valRect);
    gl::popMatrices();
}

void Oscillator::mouseDown(const cease::MouseEvent& event)
{
    compDragAnchor = event.getPos();
}

void Oscillator::mouseDrag(const cease::MouseEvent& event)
{
    canvasRect += event.getPos() - compDragAnchor;
    compDragAnchor = event.getPos();
    applyBorders();
}

bool Oscillator::isDragPoint(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    
    return titleRect.contains(local);
}

bool Oscillator::isHotspot(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    
    return titleRect.contains(local);
}

float Oscillator::getValue(int i)
{
    return val;
}

void Oscillator::setValue(int i, float v)
{
    frequency = v;
}

void Oscillator::updateVal(float newVal)
{
    val = newVal;
    valStr = getValueString();
    Vec2f valStrSize = ResourceManager::getInstance().getTextureFont()->measureString(valStr);
    valRect = Rectf(localRect.getWidth()/2 - valStrSize.x/2, localRect.getHeight()/2+3, localRect.getWidth()/2 + valStrSize.x/2, localRect.getHeight()-2);
    
    outputNodes[0]->updateVal(val);
}

std::string Oscillator::getValueString()
{
    char buff[128];
    sprintf(buff, "%." FLOAT_PRECISION "f", val);
    std::ostringstream str;
    str << buff;
    return str.str();
}
