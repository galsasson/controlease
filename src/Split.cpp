//
//  Split.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "Split.h"

Split::Split(Canvas *c) : CanvasComponent(c)
{
    type = ComponentType::COMPONENT_TYPE_SPLIT;
    immediateChange = false;
    showOutputPlus = true;
}

Split::~Split()
{
}

void Split::initNew(Vec2f pos)
{
    CanvasComponent::initNew(pos);
    setSize(Vec2f(30, 50));
    setName("Split");
    
    // add one input and two outputs to begin with
    addNewInputNode();
    addNewOutputNode();
    addNewOutputNode();
    pack(0, 0);
    
    nextVal = 0;
    updateVal(0);
}

void Split::initFromXml(XmlTree xml)
{
    
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
    gl::drawSolidRoundedRect(localRect, 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(localRect, 2);
    
    // draw title
    ResourceManager::getInstance().getTextureFont()->drawString(name, titleRect);
    gl::drawLine(Vec2f(0, titleRect.y2), Vec2f(localRect.getWidth(), titleRect.y2));
    
    // draw + sign
    gl::draw(ResourceManager::getInstance().getPlusTexture(), outputPlusRect.getUpperLeft());
    
    // draw nodes
    inputNodes[0]->draw();
    for (int i=0; i<outputNodes.size(); i++)
    {
        outputNodes[i]->draw();
    }
    
    gl::popMatrices();
}

void Split::mouseDown(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    if (outputPlusRect.contains(local)) {
        // add another output
        addNewOutputNode();
        pack(0, 0);
    }
    
    compDragAnchor = event.getPos();
}

void Split::mouseDrag(const cease::MouseEvent& event)
{
    canvasRect += event.getPos() - compDragAnchor;
    compDragAnchor = event.getPos();
    applyBorders();
}

bool Split::isDragPoint(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    
    return titleRect.contains(local);
}

bool Split::isHotspot(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    
    return titleRect.contains(local) || outputPlusRect.contains(local);
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

void Split::updateVal(float newVal)
{
    val = newVal;

    for (int i=0; i<outputNodes.size(); i++)
    {
        outputNodes[i]->updateVal(val);
    }
}
