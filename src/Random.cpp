//
//  Random.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "Random.h"

Random::Random(Vec2f p, Vec2f s)
{
    canvasRect = Rectf(p, p+s);
    initInterface(s);
    
    frequency = 0;
    
    updateVal(0);
    time = 0;
    immediateChange = false;
}

Random::~Random()
{
    delete inputNode;
    delete outputNode;
}

void Random::initInterface(Vec2f size)
{
    rect = Rectf(Vec2f(0, 0), size);
    titleRect = Rectf(2, 2, size.x-2, size.y/2-2);
    inputNode = new InputNode(0, this, Vec2f(6, size.y*3/4));
    outputNode = new OutputNode(0, this, Vec2f(size.x - 6, size.y*3/4));
}

void Random::update()
{
    if (frequency == 0 || !inputNode->isConnected()) {
        return;
    }
    
    timeval timeVal;
    gettimeofday(&timeVal, NULL);
    long millis = (timeVal.tv_sec * 1000) + (timeVal.tv_usec / 1000);
    long frameTime = millis - lastUpdate;
    
    time += frameTime;
    if (time > (float)1000 / frequency) {
        updateVal(Rand::randFloat());
        time = 0;
    }
    
    lastUpdate = millis;
}

void Random::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(rect, 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(rect, 2);
    
    // draw title
    ResourceManager::getInstance().getTextureFont()->drawString("Random", titleRect);
    gl::drawLine(Vec2f(0, rect.getHeight()/2), Vec2f(rect.getWidth(), rect.getHeight()/2));
    
    // draw nodes
    inputNode->draw();
    outputNode->draw();
    
    ResourceManager::getInstance().getTextureFont()->drawString(valStr, valRect);
    gl::popMatrices();
}

void Random::drawOutline()
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

void Random::translate(Vec2f offset)
{
    canvasRect += offset;
}

Rectf Random::getBounds()
{
    return canvasRect;
}

void Random::mouseDown(cease::MouseEvent event)
{
    compDragAnchor = event.getPos();
}

void Random::mouseDrag(cease::MouseEvent event)
{
    canvasRect += event.getPos() - compDragAnchor;
    compDragAnchor = event.getPos();
    applyBorders();
}

void Random::mouseUp( cease::MouseEvent event)
{
}

void Random::mouseWheel( cease::MouseEvent event ) {}
void Random::mouseMove( cease::MouseEvent event ) {}

bool Random::isDragPoint(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    return titleRect.contains(local);
}

bool Random::isHotspot(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    return titleRect.contains(local);
}

ConnectionResult* Random::getConnectionStart(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    if (inputNode->contains(local)) {
        if (inputNode->isConnected()) {
            return new ConnectionResult(TYPE_DISCONNECT_INPUT, inputNode);
        }
    }
    else if (outputNode->contains(local)) {
        if (outputNode->isConnected()) {
            return new ConnectionResult(TYPE_DISCONNECT_OUTPUT, outputNode);
        }
        else {
            return new ConnectionResult(TYPE_OUTPUT, outputNode);
        }
    }

    return NULL;
}

ConnectionResult* Random::getConnectionEnd(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());

    if (inputNode->contains(local)) {
        if (!inputNode->isConnected()) {
            return new ConnectionResult(TYPE_INPUT, inputNode);
        }
    }
    else if (outputNode->contains(local)) {
        if (!outputNode->isConnected()) {
            return new ConnectionResult(TYPE_OUTPUT, outputNode);
        }
    }
    
    return NULL;
}

vector<Node*> Random::getInputNodes()
{
    vector<Node*> inputs;
    inputs.push_back(inputNode);
    
    return inputs;
}

vector<Node*> Random::getOutputNodes()
{
    vector<Node*> outputs;
    outputs.push_back(outputNode);
    
    return outputs;
}


Vec2f Random::getCanvasPos()
{
    return canvasRect.getUpperLeft();
}

bool Random::contains(Vec2f p)
{
    return canvasRect.contains(p);
}

float Random::getValue(int i)
{
    return val;
}

void Random::setValue(int i, float v)
{
    frequency = v;
}

Vec2f Random::getLocalCoords(Vec2f p)
{
    return p-canvasRect.getUpperLeft();
}

Vec2f Random::getCanvasCoords(Vec2f p)
{
    return canvasRect.getUpperLeft() + p;
}

void Random::updateVal(float newVal)
{
    val = newVal;
    valStr = getValueString();
    Vec2f valStrSize = ResourceManager::getInstance().getTextureFont()->measureString(valStr);
    valRect = Rectf(rect.getWidth()/2 - valStrSize.x/2, rect.getHeight()/2+3,
                    rect.getWidth()/2 + valStrSize.x/2, rect.getHeight()-2);
    
    outputNode->updateVal(val);
}

std::string Random::getValueString()
{
    char buff[128];
    sprintf(buff, "%." FLOAT_PRECISION "f", val);
    std::ostringstream str;
    str << buff;
    return str.str();
}

void Random::applyBorders()
{
    float x1 = canvasRect.getUpperLeft().x;
    float x2 = canvasRect.getUpperRight().x;
    float y1 = canvasRect.getUpperLeft().y;
    float y2 = canvasRect.getLowerRight().y;
    
    if (x1 < 0) {
        canvasRect += Vec2f(-x1, 0);
    }
    else if (x2 > CANVAS_WIDTH) {
        canvasRect -= Vec2f(x2-CANVAS_WIDTH, 0);
    }
    
    if (y1 < 0) {
        canvasRect += Vec2f(0, -y1);
    }
    else if (y2 > CANVAS_HEIGHT) {
        canvasRect -= Vec2f(0, y2-CANVAS_HEIGHT);
    }
    
}
