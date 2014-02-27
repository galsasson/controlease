//
//  Sine.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "Sine.h"

#define TWO_PI_DIV_1000 M_PI/500

Sine::Sine(Vec2f p, Vec2f s)
{
    canvasRect = Rectf(p, p+s);
    initInterface(s);
    
    startVal = 0;
    frequency = 0;
    
    updateVal(0);
    valInc = 1;
    frequency = 0;
    time = 0;
    immediateChange = false;
}

Sine::~Sine()
{
    delete inputNode;
    delete outputNode;
}

void Sine::initInterface(Vec2f size)
{
    rect = Rectf(Vec2f(0, 0), size);
    titleRect = Rectf(2, 2, size.x-2, size.y/2-2);
    inputNode = new InputNode(0, this, Vec2f(6, size.y*3/4));
    outputNode = new OutputNode(0, this, Vec2f(size.x - 6, size.y*3/4));
    
    dragStartX = dragX = 0;
    isValDrag = false;
    isCompDrag = false;
}

void Sine::update()
{
    if (frequency == 0 || !inputNode->isConnected()) {
        return;
    }
    
    timeval timeVal;
    gettimeofday(&timeVal, NULL);
    long millis = (timeVal.tv_sec * 1000) + (timeVal.tv_usec / 1000);
    
    long frameTime = millis - lastUpdate;
    
    cout<<"duration = "<<frameTime<<endl;
    
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

void Sine::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(rect, 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(rect, 2);
    
    // draw title
    ResourceManager::getInstance().getTextureFont()->drawString("Sine", titleRect);
    gl::drawLine(Vec2f(0, rect.getHeight()/2), Vec2f(rect.getWidth(), rect.getHeight()/2));
    
    // draw nodes
    inputNode->draw();
    outputNode->draw();
    
    ResourceManager::getInstance().getTextureFont()->drawString(valStr, valRect);
    gl::popMatrices();
}

void Sine::drawOutline()
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

void Sine::translate(Vec2f offset)
{
    canvasRect += offset;
}

Rectf Sine::getBounds()
{
    return canvasRect;
}

void Sine::mouseDown(cease::MouseEvent event)
{
    compDragAnchor = event.getPos();
}

void Sine::mouseDrag(cease::MouseEvent event)
{
    canvasRect += event.getPos() - compDragAnchor;
    compDragAnchor = event.getPos();
    applyBorders();
}

void Sine::mouseUp( cease::MouseEvent event)
{
}

void Sine::mouseWheel( cease::MouseEvent event ) {}
void Sine::mouseMove( cease::MouseEvent event ) {}

bool Sine::isDragPoint(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    return titleRect.contains(local);
}

bool Sine::isHotspot(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    return titleRect.contains(local);
}

ConnectionResult* Sine::getConnectionStart(cease::MouseEvent event)
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

ConnectionResult* Sine::getConnectionEnd(cease::MouseEvent event)
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

vector<Node*> Sine::getInputNodes()
{
    vector<Node*> inputs;
    inputs.push_back(inputNode);
    
    return inputs;
}

vector<Node*> Sine::getOutputNodes()
{
    vector<Node*> outputs;
    outputs.push_back(outputNode);
    
    return outputs;
}


Vec2f Sine::getCanvasPos()
{
    return canvasRect.getUpperLeft();
}

bool Sine::contains(Vec2f p)
{
    return canvasRect.contains(p);
}

float Sine::getValue(int i)
{
    return val;
}

void Sine::setValue(int i, float v)
{
    frequency = v;
}

Vec2f Sine::getLocalCoords(Vec2f p)
{
    return p-canvasRect.getUpperLeft();
}

Vec2f Sine::getCanvasCoords(Vec2f p)
{
    return canvasRect.getUpperLeft() + p;
}

void Sine::updateVal(float newVal)
{
    val = newVal;
    valStr = getValueString();
    Vec2f valStrSize = ResourceManager::getInstance().getTextureFont()->measureString(valStr);
    valRect = Rectf(rect.getWidth()/2 - valStrSize.x/2, rect.getHeight()/2+3,
                    rect.getWidth()/2 + valStrSize.x/2, rect.getHeight()-2);
    
    outputNode->updateVal(val);
}

std::string Sine::getValueString()
{
    char buff[128];
    sprintf(buff, "%." FLOAT_PRECISION "f", val);
    std::ostringstream str;
    str << buff;
    return str.str();
}

void Sine::applyBorders()
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
