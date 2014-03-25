//
//  Oscillator.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "Oscillator.h"

#define TWO_PI_DIV_1000 M_PI/500

Oscillator::Oscillator(Canvas *c, Vec2f p, Vec2f s) : CanvasComponent(c)
{
    canvasRect = Rectf(p, p+s);
    initInterface(s);
    
    frequency = 0;
    
    updateVal(0);
    time = 0;
    immediateChange = false;
}

Oscillator::~Oscillator()
{
}

void Oscillator::initInterface(Vec2f size)
{
    rect = Rectf(Vec2f(0, 0), size);
    titleRect = Rectf(2, 2, size.x-2, size.y/2-2);
    inputNodes.push_back(new InputNode(0, this, Vec2f(6, size.y*3/4)));
    inputNodes[0]->setName("Frequency");
    outputNodes.push_back(new OutputNode(0, this, Vec2f(size.x - 6, size.y*3/4)));
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
    
    updateVal(sin(time));
    
    lastUpdate = millis;
}

void Oscillator::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(rect, 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(rect, 2);
    
    // draw title
    ResourceManager::getInstance().getTextureFont()->drawString("Oscillator", titleRect);
    gl::drawLine(Vec2f(0, rect.getHeight()/2), Vec2f(rect.getWidth(), rect.getHeight()/2));
    
    // draw nodes
    inputNodes[0]->draw();
    outputNodes[0]->draw();
    
    ResourceManager::getInstance().getTextureFont()->drawString(valStr, valRect);
    gl::popMatrices();
}

void Oscillator::drawOutline()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft() - Vec2f(4, 4));

    glPushAttrib(GL_ENABLE_BIT);
    glLineStipple(1, 0xff00);
    gl::enable(GL_LINE_STIPPLE);
    
    gl::color(0, 0, 0);
    gl::drawStrokedRect(Rectf(Vec2f(0, 0), canvasRect.getSize() + Vec2f(8, 8)));
    
    glPopAttrib();
    
    gl::popMatrices();
}

void Oscillator::translate(Vec2f offset)
{
    canvasRect += offset;
}

Rectf Oscillator::getBounds()
{
    return canvasRect;
}

void Oscillator::mouseDown(cease::MouseEvent event)
{
    compDragAnchor = event.getPos();
}

void Oscillator::mouseDrag(cease::MouseEvent event)
{
    canvasRect += event.getPos() - compDragAnchor;
    compDragAnchor = event.getPos();
    applyBorders();
}

void Oscillator::mouseUp( cease::MouseEvent event)
{
}

void Oscillator::mouseWheel( cease::MouseEvent event ) {}
void Oscillator::mouseMove( cease::MouseEvent event ) {}

bool Oscillator::isDragPoint(cease::MouseEvent event)
{
    Vec2f local = toLocal(event.getPos());
    
    return titleRect.contains(local);
}

bool Oscillator::isHotspot(cease::MouseEvent event)
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
    valRect = Rectf(rect.getWidth()/2 - valStrSize.x/2, rect.getHeight()/2+3,
                    rect.getWidth()/2 + valStrSize.x/2, rect.getHeight()-2);
    
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
