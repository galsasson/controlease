//
//  CanvasComponent.cpp
//  Controlease
//
//  Created by Gal Sasson on 3/22/14.
//
//

#include "CanvasComponent.h"

CanvasComponent::~CanvasComponent() {
    for (int i=0; i<inputNodes.size(); i++) {
        delete inputNodes[i];
    }
    for (int i=0; i<outputNodes.size(); i++) {
        delete outputNodes[i];
    }
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

ConnectionResult* CanvasComponent::getConnectionStart(cease::MouseEvent event)
{
    Vec2f local = toLocal(event.getPos());
    
    for (int i=0; i<inputNodes.size(); i++)
    {
        if (inputNodes[i]->contains(local)) {
            if (inputNodes[i]->isConnected()) {
                return new ConnectionResult(TYPE_DISCONNECT_INPUT, inputNodes[i]);
            }
        }
    }
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
    
    return NULL;
}

ConnectionResult* CanvasComponent::getConnectionEnd(cease::MouseEvent event)
{
    Vec2f local = toLocal(event.getPos());
    
    for (int i=0; i<inputNodes.size(); i++)
    {
        if (inputNodes[i]->contains(local)) {
            if (!inputNodes[i]->isConnected()) {
                return new ConnectionResult(TYPE_INPUT, inputNodes[i]);
            }
        }
    }
    for (int i=0; i<outputNodes.size(); i++)
    {
        if (outputNodes[i]->contains(local)) {
            if (!outputNodes[i]->isConnected()) {
                return new ConnectionResult(TYPE_OUTPUT, outputNodes[i]);
            }
        }
    }
    
    return NULL;
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

void CanvasComponent::translate(Vec2f offset)
{
    canvasRect += offset;
}

Rectf CanvasComponent::getBounds()
{
    return canvasRect;
}

Vec2f CanvasComponent::getCanvasPos()
{
    return canvasRect.getUpperLeft();
}

bool CanvasComponent::contains(Vec2f canvasPoint)
{
    return canvasRect.contains(canvasPoint);
}

Vec2f CanvasComponent::toLocal(Vec2f p)
{
    return p-canvasRect.getUpperLeft();
}

Vec2f CanvasComponent::toCanvas(Vec2f p)
{
    return canvasRect.getUpperLeft() + p;
}
