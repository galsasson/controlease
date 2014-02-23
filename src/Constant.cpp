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
    
    startVal = 0;
    nextVal = 0;
    updateVal(0);
    valInc = 1;
    
    immediateChange = false;
}

Constant::~Constant()
{
    delete inputNode;
    delete outputNode;
}

void Constant::initInterface(Vec2f size)
{
    rect = Rectf(Vec2f(0, 0), size);
    Vec2f titleSize = ResourceManager::getInstance().getTextureFont()->measureString("Constant");
    titleRect = Rectf(size.x/2 - titleSize.x/2, 2, size.x/2 + titleSize.x/2, size.y/2-2);
    inputNode = new InputNode(0, this, Vec2f(6, size.y*3/4));
    outputNode = new OutputNode(0, this, Vec2f(size.x - 6, size.y*3/4));
    
    dragStartX = dragX = 0;
    isValDrag = false;
    isCompDrag = false;
}

void Constant::update()
{
    if (!immediateChange)
    {
        if (nextVal != val) {
            updateVal(nextVal);
        }
    }
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
    inputNode->draw();
    outputNode->draw();
    
    ResourceManager::getInstance().getTextureFont()->drawString(valStr, valRect);
    gl::popMatrices();
}

void Constant::drawOutline()
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
}

void Constant::mouseDrag(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    if (isValDrag) {
        dragX = local.x;
        if (immediateChange) {
            updateVal(startVal + (dragX - dragStartX) * valInc);
        }
        else {
            nextVal = startVal + (dragX - dragStartX) * valInc;
        }
    }

    if (isCompDrag) {
        canvasRect += event.getPos() - compDragAnchor;
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

ConnectionResult* Constant::getConnectionStart(cease::MouseEvent event)
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

ConnectionResult* Constant::getConnectionEnd(cease::MouseEvent event)
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

vector<Node*> Constant::getInputNodes()
{
    vector<Node*> inputs;
    inputs.push_back(inputNode);
    
    return inputs;
}

vector<Node*> Constant::getOutputNodes()
{
    vector<Node*> outputs;
    outputs.push_back(outputNode);
    
    return outputs;
}


Vec2f Constant::getCanvasPos()
{
    return canvasRect.getUpperLeft();
}

bool Constant::contains(Vec2f p)
{
    return canvasRect.contains(p);
}

float Constant::getValue(int i)
{
    return val;
}

void Constant::setValue(int i, float v)
{
    if (immediateChange) {
        updateVal(v);
    }
    else {
        nextVal = v;
    }
}

Vec2f Constant::getLocalCoords(Vec2f p)
{
    return p-canvasRect.getUpperLeft();
}

Vec2f Constant::getCanvasCoords(Vec2f p)
{
    return canvasRect.getUpperLeft() + p;
}

void Constant::updateVal(float newVal)
{
    val = newVal;
    valStr = getValueString();
//    console() << valStr << endl;
    Vec2f valStrSize = ResourceManager::getInstance().getTextureFont()->measureString(valStr);
    valRect = Rectf(rect.getWidth()/2 - valStrSize.x/2, rect.getHeight()/2+3,
                    rect.getWidth()/2 + valStrSize.x/2, rect.getHeight()-2);
    
    outputNode->updateVal(val);
}

std::string Constant::getValueString()
{
    char buff[128];
    sprintf(buff, "%." FLOAT_PRECISION "f", val);
    std::ostringstream str;
    str << buff;
    return str.str();
}
