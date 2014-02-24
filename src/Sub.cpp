//
//  Sub.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "Sub.h"

Sub::Sub(Vec2f p, Vec2f s)
{
    canvasRect = Rectf(p, p+s);
    initInterface(s);
    
    vals.push_back(0);
    vals.push_back(0);
    nextVal = 0;
    updateVal(0);
    valInc = 1;
    
    immediateChange = false;
}

Sub::~Sub()
{
    for (int i=0; i<inputNodes.size(); i++)
    {
        delete inputNodes[i];
    }
    delete outputNode;
}

void Sub::initInterface(Vec2f size)
{
    rect = Rectf(Vec2f(0, 0), size);
    titleRect = Rectf(2, 2, size.x-20, 20);
    for (int i=0; i<2; i++)
    {
        inputNodes.push_back(new InputNode(i, this, Vec2f(6, titleRect.y2 + 6 + i*9)));
    }
    outputNode = new OutputNode(0, this, Vec2f(size.x-6, size.y*3/4));
}

void Sub::update()
{
    if (!immediateChange)
    {
        if (nextVal != val) {
            updateVal(nextVal);
        }
    }
}

void Sub::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(rect, 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(rect, 2);
    
    // draw title
    ResourceManager::getInstance().getTextureFont()->drawString("Subtract", titleRect);
    gl::drawLine(Vec2f(0, titleRect.y2), Vec2f(rect.getWidth(), titleRect.y2));
    
    // draw nodes
    outputNode->draw();
    for (int i=0; i<inputNodes.size(); i++)
    {
        inputNodes[i]->draw();
    }
    
    ResourceManager::getInstance().getTextureFont()->drawString(valStr, valRect);
    gl::popMatrices();
}

void Sub::drawOutline()
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

void Sub::translate(Vec2f offset)
{
    canvasRect += offset;
}

Rectf Sub::getBounds()
{
    return canvasRect;
}

void Sub::mouseDown(cease::MouseEvent event)
{
    compDragAnchor = event.getPos();
}

void Sub::mouseDrag(cease::MouseEvent event)
{
    canvasRect += event.getPos() - compDragAnchor;
    compDragAnchor = event.getPos();
    applyBorders();
}

void Sub::mouseUp( cease::MouseEvent event)
{
}

void Sub::mouseWheel( cease::MouseEvent event ) {}
void Sub::mouseMove( cease::MouseEvent event ) {}

bool Sub::isDragPoint(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    return titleRect.contains(local);
}

bool Sub::isHotspot(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    return titleRect.contains(local);
}

ConnectionResult* Sub::getConnectionStart(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    for (int i=0; i<inputNodes.size(); i++)
    {
        if (inputNodes[i]->contains(local)) {
            if (inputNodes[i]->isConnected()) {
                return new ConnectionResult(TYPE_DISCONNECT_INPUT, inputNodes[i]);
            }
        }
    }
    
    if (outputNode->contains(local)) {
        if (outputNode->isConnected()) {
            return new ConnectionResult(TYPE_DISCONNECT_OUTPUT, outputNode);
        }
        else {
            return new ConnectionResult(TYPE_OUTPUT, outputNode);
        }
    }

    return NULL;
}

ConnectionResult* Sub::getConnectionEnd(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());

    for (int i=0; i<inputNodes.size(); i++)
    {
        if (inputNodes[i]->contains(local)) {
            if (!inputNodes[i]->isConnected()) {
                return new ConnectionResult(TYPE_INPUT, inputNodes[i]);
            }
        }
    }
    
    if (outputNode->contains(local)) {
        if (!outputNode->isConnected()) {
            return new ConnectionResult(TYPE_OUTPUT, outputNode);
        }
    }
    
    return NULL;
}

vector<Node*> Sub::getInputNodes()
{
    vector<Node*> inputs;
    
    for (int i=0; i<inputNodes.size(); i++)
    {
        inputs.push_back((Node*)inputNodes[i]);
    }
    
    return inputs;
}

vector<Node*> Sub::getOutputNodes()
{
    vector<Node*> outputs;
    
    outputs.push_back((Node*)outputNode);
    
    return outputs;
}


Vec2f Sub::getCanvasPos()
{
    return canvasRect.getUpperLeft();
}

bool Sub::contains(Vec2f p)
{
    return canvasRect.contains(p);
}

float Sub::getValue(int i)
{
    return val;
}

void Sub::setValue(int i, float v)
{
    vals[i] = v;
    nextVal = vals[0];
    for (int i=1; i<vals.size(); i++)
    {
        nextVal -= vals[i];
    }

    if (immediateChange) {
        updateVal(nextVal);
    }
}

Vec2f Sub::getLocalCoords(Vec2f p)
{
    return p-canvasRect.getUpperLeft();
}

Vec2f Sub::getCanvasCoords(Vec2f p)
{
    return canvasRect.getUpperLeft() + p;
}

void Sub::updateVal(float newVal)
{
    val = newVal;
    valStr = getValueString();
    Vec2f valStrSize = ResourceManager::getInstance().getTextureFont()->measureString(valStr);
    valRect = Rectf(rect.getWidth()/2 - valStrSize.x/2, rect.getHeight()/2+3,
                    rect.getWidth()/2 + valStrSize.x/2, rect.getHeight()-2);
    
    outputNode->updateVal(val);
}

std::string Sub::getValueString()
{
    char buff[128];
    sprintf(buff, "%." FLOAT_PRECISION "f", val);
    std::ostringstream str;
    str << buff;
    return str.str();
}

void Sub::applyBorders()
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
