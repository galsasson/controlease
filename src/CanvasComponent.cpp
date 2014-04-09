//
//  CanvasComponent.cpp
//  Controlease
//
//  Created by Gal Sasson on 3/22/14.
//
//

#include "CanvasComponent.h"

CanvasComponent::CanvasComponent(Canvas *c)
{
    canvas = c;
    
    showInputPlus = false;
    showOutputPlus = false;
}

CanvasComponent::~CanvasComponent() {
    for (int i=0; i<inputNodes.size(); i++) {
        delete inputNodes[i];
    }
    for (int i=0; i<outputNodes.size(); i++) {
        delete outputNodes[i];
    }
}

void CanvasComponent::initNew(Vec2f p)
{
    originalSize = Vec2f(0, 0);
    canvasRect = Rectf(p, p+originalSize);
}

void CanvasComponent::initFromXml(const XmlTree& xml)
{
    setName(xml.getAttributeValue<std::string>("name"));
    Vec2f pos = Vec2f(xml.getAttributeValue<float>("position.x"), xml.getAttributeValue<float>("position.y"));
    Vec2f size = Vec2f(xml.getAttributeValue<float>("size.x"), xml.getAttributeValue<float>("size.y"));
    canvasRect = Rectf(pos, pos+size);
    
    setSize(size);
    
    showInputPlus = xml.getAttributeValue<bool>("showInputPlus");
    showOutputPlus = xml.getAttributeValue<bool>("showOutputPlus");
    
    // add inputs and outputs
    XmlTree inputNodesTree = xml.getChild("InputNodes");
    for(XmlTree::ConstIter iter = inputNodesTree.begin(); iter != inputNodesTree.end(); ++iter)
    {
        if (iter->getTag() == "Node") {
            addInputNodeFromXml(iter->getChild(""));
        }
    }
    XmlTree outputNodesTree = xml.getChild("OutputNodes");
    for(XmlTree::ConstIter iter = outputNodesTree.begin(); iter != outputNodesTree.end(); ++iter)
    {
        if (iter->getTag() == "Node") {
            addOutputNodeFromXml(iter->getChild(""));
        }
    }
}


void CanvasComponent::setSize(Vec2f size)
{
    originalSize = size;
    
    canvasRect.x2 = canvasRect.x1 + size.x;
    canvasRect.y2 = canvasRect.y1 + size.y;
    localRect = Rectf(Vec2f(0, 0), size);
    titleRect = Rectf(2, 2, size.x-2, 20);
    
    nextInputPos = Vec2f(6, titleRect.y2 + 8);
    nextOutputPos = Vec2f(size.x - 6, titleRect.y2 + 8);
    inputPlusRect = Rectf(nextInputPos - Vec2f(4, 3), nextInputPos + Vec2f(4, 5));
    outputPlusRect = Rectf(nextOutputPos - Vec2f(4, 3), nextOutputPos + Vec2f(4, 5));
}

void CanvasComponent::setName(std::string n)
{
    name = n;
    nameSize = ResourceManager::getInstance().getTextureFont()->measureString(name);
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

Node* CanvasComponent::getNodeBelow(const cease::MouseEvent& event)
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

ConnectionResult CanvasComponent::getConnectionStart(const cease::MouseEvent& event)
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

ConnectionResult CanvasComponent::getConnectionEnd(const cease::MouseEvent& event)
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
        if (id == inputNodes[i]->getId()) {
            return inputNodes[i];
        }
    }
    
    for (int i=0; i<outputNodes.size(); i++)
    {
        if (id == outputNodes[i]->getId()) {
            return outputNodes[i];
        }
    }
    
    return NULL;
}

void CanvasComponent::translate(const Vec2f& offset)
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

bool CanvasComponent::contains(const Vec2f& canvasPoint)
{
    return canvasRect.contains(canvasPoint);
}

void CanvasComponent::pack(float minX, float minY)
{
    float neededHeight = max(nextInputPos.y, nextOutputPos.y);
    if (showInputPlus || showOutputPlus) {
        neededHeight += 9;
    }
    
    float neededWidth = max(nameSize.x, originalSize.x);
    
    neededWidth = max(neededWidth, minX);
    neededHeight = max(neededHeight, minY);
    
    canvasRect.x2 = canvasRect.x1 + neededWidth;
    canvasRect.y2 = canvasRect.y1 + neededHeight;
    localRect.x2 = localRect.x1 + neededWidth;
    localRect.y2 = localRect.y1 + neededHeight;
    titleRect.x2 = titleRect.x1 + neededWidth;
    
    // update node positions
    nextOutputPos.x = localRect.getWidth() - 6;
    for (int i=0; i<outputNodes.size(); i++)
    {
        outputNodes[i]->getPosition().x = localRect.getWidth() - 6;
    }
    outputPlusRect = Rectf(nextOutputPos - Vec2f(4, 3), nextOutputPos + Vec2f(4, 5));
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
            return "Javascript";
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
    cComp.setAttribute("position.x", canvasRect.x1);
    cComp.setAttribute("position.y", canvasRect.y1);
    cComp.setAttribute("size.x", localRect.getWidth());
    cComp.setAttribute("size.y", localRect.getHeight());
    cComp.setAttribute("showInputPlus", showInputPlus);
    cComp.setAttribute("showOutputPlus", showOutputPlus);
    
    XmlTree inodes("InputNodes", "");
    for (int i=0; i<inputNodes.size(); i++)
    {
        inodes.push_back(inputNodes[i]->getXml());
    }
    cComp.push_back(inodes);
    
    XmlTree onodes("OutputNodes", "");
    for (int i=0; i<outputNodes.size(); i++)
    {
        onodes.push_back(outputNodes[i]->getXml());
    }
    cComp.push_back(onodes);
    
    return cComp;
}

Vec2f CanvasComponent::toLocal(const Vec2f& p)
{
    return p-canvasRect.getUpperLeft();
}

Vec2f CanvasComponent::toCanvas(const Vec2f& p)
{
    return canvasRect.getUpperLeft() + p;
}

InputNode* CanvasComponent::addNewInputNode()
{
    InputNode* node = new InputNode(this);
    node->initNew(inputNodes.size(), nextInputPos);
    inputNodes.push_back(node);
    nextInputPos.y += 9;
    inputPlusRect += Vec2f(0, 9);
    
    return node;
}

OutputNode* CanvasComponent::addNewOutputNode()
{
    OutputNode* node = new OutputNode(this);
    node->initNew(outputNodes.size(), nextOutputPos);
    outputNodes.push_back(node);
    nextOutputPos.y += 9;
    outputPlusRect += Vec2f(0, 9);
    
    return node;
}

InputNode* CanvasComponent::addInputNodeFromXml(const cinder::XmlTree &xml)
{
    InputNode* node = new InputNode(this);
    node->initFromXml(xml);
    inputNodes.push_back(node);
    nextInputPos.y += 9;
    inputPlusRect += Vec2f(0, 9);
    
    return node;
}

OutputNode* CanvasComponent::addOutputNodeFromXml(const cinder::XmlTree &xml)
{
    OutputNode* node = new OutputNode(this);
    node->initFromXml(xml);
    outputNodes.push_back(node);
    nextOutputPos.y += 9;
    outputPlusRect += Vec2f(0, 9);
    
    return node;
}
