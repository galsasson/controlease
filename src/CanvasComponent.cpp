//
//  CanvasComponent.cpp
//  Controlease
//
//  Created by Gal Sasson on 3/22/14.
//
//

#include "CanvasComponent.h"

CanvasComponent::CanvasComponent(Canvas *c, ComponentType t)
{
    canvas = c;
    type = t;
}

CanvasComponent::~CanvasComponent() {
    for (int i=0; i<inputNodes.size(); i++) {
        delete inputNodes[i];
    }
    for (int i=0; i<outputNodes.size(); i++) {
        delete outputNodes[i];
    }
}

void CanvasComponent::drawOutline()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft() - Vec2f(4, 4));
    
    glPushAttrib(GL_ENABLE_BIT);
    glLineStipple(1, 0xff00);
    gl::enable(GL_LINE_STIPPLE);
    
    gl::color(0.5, 0.5, 0.5);
    gl::drawStrokedRect(Rectf(Vec2f(0, 0), canvasRect.getSize() + Vec2f(8, 8)));
    
    glPopAttrib();
    
    gl::popMatrices();
}

Node* CanvasComponent::getNodeBelow(cease::MouseEvent event)
{
    for (int i=0; i<inputNodes.size(); i++)
    {
        Vec2f distance = event.getPos() - inputNodes[i]->getCanvasPos();
        if (distance.length() < 5) {
            return inputNodes[i];
        }
    }
    
    for (int i=0; i<outputNodes.size(); i++)
    {
        Vec2f distance = event.getPos() - outputNodes[i]->getCanvasPos();
        if (distance.length() < 5) {
            return outputNodes[i];
        }
    }
    
    return NULL;
}

ConnectionResult CanvasComponent::getConnectionStart(cease::MouseEvent event)
{
    Vec2f local = toLocal(event.getPos());
    
    for (int i=0; i<inputNodes.size(); i++)
    {
        if (inputNodes[i]->contains(local)) {
            if (inputNodes[i]->isConnected()) {
                return ConnectionResult(TYPE_DISCONNECT_INPUT, inputNodes[i]);
            }
        }
    }
    for (int i=0; i<outputNodes.size(); i++)
    {
        if (outputNodes[i]->contains(local)) {
            if (outputNodes[i]->isConnected()) {
                return ConnectionResult(TYPE_DISCONNECT_OUTPUT, outputNodes[i]);
            }
            else {
                return ConnectionResult(TYPE_OUTPUT, outputNodes[i]);
            }
        }
    }
    
    return ConnectionResult();
}

ConnectionResult CanvasComponent::getConnectionEnd(cease::MouseEvent event)
{
    Vec2f local = toLocal(event.getPos());
    
    for (int i=0; i<inputNodes.size(); i++)
    {
        if (inputNodes[i]->contains(local)) {
            if (!inputNodes[i]->isConnected()) {
                return ConnectionResult(TYPE_INPUT, inputNodes[i]);
            }
        }
    }
    for (int i=0; i<outputNodes.size(); i++)
    {
        if (outputNodes[i]->contains(local)) {
            if (!outputNodes[i]->isConnected()) {
                return ConnectionResult(TYPE_OUTPUT, outputNodes[i]);
            }
        }
    }
    
    return ConnectionResult();
}

vector<Node*> CanvasComponent::getInputNodes()
{
    vector<Node*> inputs;
    
    for (int i=0; i<inputNodes.size(); i++) {
        inputs.push_back((Node*)inputNodes[i]);
    }
    
    return inputs;
}

vector<Node*> CanvasComponent::getOutputNodes()
{
    vector<Node*> outputs;
    
    for (int i=0; i<outputNodes.size(); i++) {
        outputs.push_back((Node*)outputNodes[i]);
    }
    
    return outputs;
}

Node* CanvasComponent::getNodeWithID(int id)
{
    for (int i=0; i<inputNodes.size(); i++)
    {
        if (id == inputNodes[i]->id) {
            return inputNodes[i];
        }
    }
    
    for (int i=0; i<outputNodes.size(); i++)
    {
        if (id == outputNodes[i]->id) {
            return outputNodes[i];
        }
    }
    
    return NULL;
}

void CanvasComponent::translate(Vec2f offset)
{
    canvasRect += offset;
}

Rectf CanvasComponent::getBounds()
{
    return canvasRect;
}

void CanvasComponent::applyBorders()
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

Vec2f CanvasComponent::getCanvasPos()
{
    return canvasRect.getUpperLeft();
}

bool CanvasComponent::contains(Vec2f canvasPoint)
{
    return canvasRect.contains(canvasPoint);
}

void CanvasComponent::setName(std::string n)
{
    name = n;
    nameSize = ResourceManager::getInstance().getTextureFont()->measureString(name);
}

string CanvasComponent::getComponentTypeString(ComponentType t)
{
    switch (t)
    {
        case COMPONENT_TYPE_PROGRAM:
            return "Program";
        case COMPONENT_TYPE_NUMBER:
            return "Number";
        case COMPONENT_TYPE_SPLIT:
            return "Split";
        case COMPONENT_TYPE_OSCILLATOR:
            return "Oscillator";
        case COMPONENT_TYPE_EXP:
            return "Expression";
        case COMPONENT_TYPE_JS:
        {
            return "Javascript";
        }
        case COMPONENT_TYPE_OSCCONTROLLER:
            return "OscController";
    }
    
    return "Unknown";
}

string CanvasComponent::getJSComponentTypeString(std::string scriptFile)
{
    fs::path fileNoExt(scriptFile);
    fileNoExt = fileNoExt.replace_extension("").filename();
    return fileNoExt.string();
}

XmlTree CanvasComponent::getXml()
{
    XmlTree cComp("CanvasComponent", "");
    cComp.setAttribute("type", ci::toString(type));
    cComp.setAttribute("name", name);
    cComp.setAttribute("nameSize", nameSize);
    cComp.setAttribute("canvasRectUL", canvasRect.getUpperLeft());
    cComp.setAttribute("canvasRectLR", canvasRect.getLowerRight());
    
    XmlTree inodes("InputNodes", "");
    for (int i=0; i<inputNodes.size(); i++)
    {
        inodes.push_back(inputNodes[i]->getXml());
    }
    cComp.push_back(inodes);
    
    XmlTree onodes("OutputNodes", "");
    for (int i=0; i<outputNodes.size(); i++)
    {
        cComp.push_back(outputNodes[i]->getXml());
    }
    cComp.push_back(onodes);
    
    return cComp;
}

Vec2f CanvasComponent::toLocal(Vec2f p)
{
    return p-canvasRect.getUpperLeft();
}

Vec2f CanvasComponent::toCanvas(Vec2f p)
{
    return canvasRect.getUpperLeft() + p;
}
