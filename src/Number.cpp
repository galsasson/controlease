//
//  Number.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "Number.h"

Number::Number(Vec2f p, Vec2f s)
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
    delete inputNode;
    delete outputNode;
}

void Number::initInterface(Vec2f size)
{
    rect = Rectf(Vec2f(0, 0), size);
    titleRect = Rectf(2, 2, size.x-2, size.y/2-2);
    inputNode = new InputNode(0, this, Vec2f(6, size.y*3/4));
    outputNode = new OutputNode(0, this, Vec2f(size.x - 6, size.y*3/4));
    
    dragStartX = dragX = 0;
    isValDrag = false;
    isCompDrag = false;
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
    ResourceManager::getInstance().getTextureFont()->drawString("Number", titleRect);
    gl::drawLine(Vec2f(0, rect.getHeight()/2), Vec2f(rect.getWidth(), rect.getHeight()/2));
    
    // draw nodes
    inputNode->draw();
    outputNode->draw();
    
    ResourceManager::getInstance().getTextureFont()->drawString(valStr, valRect);
    gl::popMatrices();
}

void Number::drawOutline()
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

void Number::translate(Vec2f offset)
{
    canvasRect += offset;
}

Rectf Number::getBounds()
{
    return canvasRect;
}

void Number::mouseDown(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
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

void Number::mouseDrag(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
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

void Number::mouseUp( cease::MouseEvent event)
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

void Number::mouseWheel( cease::MouseEvent event ) {}
void Number::mouseMove( cease::MouseEvent event ) {}

bool Number::isDragPoint(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    return valRect.contains(local) || titleRect.contains(local);
}

bool Number::isHotspot(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    return valRect.contains(local) || titleRect.contains(local);
}

ConnectionResult* Number::getConnectionStart(cease::MouseEvent event)
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

ConnectionResult* Number::getConnectionEnd(cease::MouseEvent event)
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

vector<Node*> Number::getInputNodes()
{
    vector<Node*> inputs;
    inputs.push_back(inputNode);
    
    return inputs;
}

vector<Node*> Number::getOutputNodes()
{
    vector<Node*> outputs;
    outputs.push_back(outputNode);
    
    return outputs;
}

KeyboardListener* Number::getCurrentKeyboardListener()
{
    return NULL;
}

Vec2f Number::getCanvasPos()
{
    return canvasRect.getUpperLeft();
}

bool Number::contains(Vec2f p)
{
    return canvasRect.contains(p);
}

float Number::getValue(int i)
{
    return val;
}

void Number::setValue(int i, float v)
{
    if (immediateChange) {
        updateVal(v);
    }
    else {
        nextVal = v;
    }
}

Vec2f Number::getLocalCoords(Vec2f p)
{
    return p-canvasRect.getUpperLeft();
}

Vec2f Number::getCanvasCoords(Vec2f p)
{
    return canvasRect.getUpperLeft() + p;
}

void Number::updateVal(float newVal)
{
    val = newVal;
    valStr = getValueString();
    Vec2f valStrSize = ResourceManager::getInstance().getTextureFont()->measureString(valStr);
    valRect = Rectf(rect.getWidth()/2 - valStrSize.x/2, rect.getHeight()/2+3,
                    rect.getWidth()/2 + valStrSize.x/2, rect.getHeight()-2);
    
    outputNode->updateVal(val);
}

std::string Number::getValueString()
{
    char buff[128];
    sprintf(buff, "%." FLOAT_PRECISION "f", val);
    std::ostringstream str;
    str << buff;
    return str.str();
}

void Number::applyBorders()
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
