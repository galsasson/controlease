//
//  Split.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "Split.h"

Split::Split(Vec2f p, Vec2f s)
{
    canvasRect = Rectf(p, p+s);
    initInterface(s);
    
    nextVal = 0;
    updateVal(0);
    valInc = 1;
    
    immediateChange = false;
}

Split::~Split()
{
    delete inputNode;
    for (int i=0; i<outputNodes.size(); i++)
    {
        delete outputNodes[i];
    }
}

void Split::initInterface(Vec2f size)
{
    rect = Rectf(Vec2f(0, 0), size);
    titleRect = Rectf(2, 2, size.x-20, 20);
    plusRect = Rectf(size.x-12, 5, size.x-2, 15);
    inputNode = new InputNode(0, this, Vec2f(6, size.y*3/4));
    for (int i=0; i<2; i++)
    {
        outputNodes.push_back(new OutputNode(i, this, Vec2f(size.x - 6, titleRect.y2 + 6 + i*9)));
    }
    nextOutputPos = Vec2f(size.x - 6, titleRect.y2 + 24);
}

void Split::update()
{
    if (!immediateChange)
    {
        if (nextVal != val) {
            updateVal(nextVal);
        }
    }
}

void Split::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(rect, 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(rect, 2);
    
    // draw title
    ResourceManager::getInstance().getTextureFont()->drawString("Split", titleRect);
    gl::drawLine(Vec2f(0, titleRect.y2), Vec2f(rect.getWidth(), titleRect.y2));
    
    // draw + sign
    gl::drawLine(Vec2f(plusRect.x1, plusRect.y1 + plusRect.getHeight()/2), Vec2f(plusRect.x2, plusRect.y1 + plusRect.getHeight()/2));
    gl::drawLine(Vec2f(plusRect.x1 + plusRect.getWidth()/2, plusRect.y1), Vec2f(plusRect.x1 + plusRect.getWidth()/2, plusRect.y2));
    
    // draw nodes
    inputNode->draw();
    for (int i=0; i<outputNodes.size(); i++)
    {
        outputNodes[i]->draw();
    }
    
    ResourceManager::getInstance().getTextureFont()->drawString(valStr, valRect);
    gl::popMatrices();
}

void Split::drawOutline()
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

void Split::translate(Vec2f offset)
{
    canvasRect += offset;
}

Rectf Split::getBounds()
{
    return canvasRect;
}

void Split::mouseDown(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    if (plusRect.contains(local)) {
        // add another output
        outputNodes.push_back(new OutputNode(outputNodes.size(), this, nextOutputPos));
        nextOutputPos.y += 9;
        canvasRect.y2 += 9;
        rect.y2 += 9;
    }
    
    compDragAnchor = event.getPos();
}

void Split::mouseDrag(cease::MouseEvent event)
{
    canvasRect += event.getPos() - compDragAnchor;
    compDragAnchor = event.getPos();
    applyBorders();
}

void Split::mouseUp( cease::MouseEvent event)
{
}

void Split::mouseWheel( cease::MouseEvent event ) {}
void Split::mouseMove( cease::MouseEvent event ) {}

bool Split::isDragPoint(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    return titleRect.contains(local);
}

bool Split::isHotspot(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    return titleRect.contains(local) || plusRect.contains(local);
}

ConnectionResult* Split::getConnectionStart(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    if (inputNode->contains(local)) {
        if (inputNode->isConnected()) {
            return new ConnectionResult(TYPE_DISCONNECT_INPUT, inputNode);
        }
    }
    else {
        for (int i=0; i<outputNodes.size(); i++)
        {
            if (outputNodes[i]->contains(local)) {
                if (outputNodes[i]->isConnected()) {
                    return new ConnectionResult(TYPE_DISCONNECT_OUTPUT, outputNodes[i]);
                }
                else {
                    return new ConnectionResult(TYPE_OUTPUT, outputNodes[i]);
                }
            }
        }
    }

    return NULL;
}

ConnectionResult* Split::getConnectionEnd(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());

    if (inputNode->contains(local)) {
        if (!inputNode->isConnected()) {
            return new ConnectionResult(TYPE_INPUT, inputNode);
        }
    }
    else {
        for (int i=0; i<outputNodes.size(); i++)
        {
            if (outputNodes[i]->contains(local)) {
                if (!outputNodes[i]->isConnected()) {
                    return new ConnectionResult(TYPE_OUTPUT, outputNodes[i]);
                }
            }
        }
    }
    
    return NULL;
}

vector<Node*> Split::getInputNodes()
{
    vector<Node*> inputs;
    inputs.push_back(inputNode);
    
    return inputs;
}

vector<Node*> Split::getOutputNodes()
{
    vector<Node*> outputs;
    
    for (int i=0; i<outputNodes.size(); i++)
    {
        outputs.push_back((Node*)outputNodes[i]);
    }
    
    return outputs;
}


Vec2f Split::getCanvasPos()
{
    return canvasRect.getUpperLeft();
}

bool Split::contains(Vec2f p)
{
    return canvasRect.contains(p);
}

float Split::getValue(int i)
{
    return val;
}

void Split::setValue(int i, float v)
{
    if (immediateChange) {
        updateVal(v);
    }
    else {
        nextVal = v;
    }
}

Vec2f Split::getLocalCoords(Vec2f p)
{
    return p-canvasRect.getUpperLeft();
}

Vec2f Split::getCanvasCoords(Vec2f p)
{
    return canvasRect.getUpperLeft() + p;
}

void Split::updateVal(float newVal)
{
    val = newVal;
    valStr = getValueString();
    Vec2f valStrSize = ResourceManager::getInstance().getTextureFont()->measureString(valStr);
    valRect = Rectf(rect.getWidth()/2 - valStrSize.x/2, rect.getHeight()/2+3,
                    rect.getWidth()/2 + valStrSize.x/2, rect.getHeight()-2);
    
    for (int i=0; i<outputNodes.size(); i++)
    {
        outputNodes[i]->updateVal(val);
    }
}

std::string Split::getValueString()
{
    char buff[128];
    sprintf(buff, "%." FLOAT_PRECISION "f", val);
    std::ostringstream str;
    str << buff;
    return str.str();
}

void Split::applyBorders()
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
