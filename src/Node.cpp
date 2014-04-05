//
//  Node.cpp
//  Controlease
//
//  Created by Gal Sasson on 3/24/14.
//
//

#include "Node.h"

static int globalNodeID = 0;

Node::Node(int i, CanvasComponent *comp, Vec2f p)
{
    id = globalNodeID++;

    index = i;
    component = comp;
    pos = p;
    
    bDrawActive = false;
    
    next = NULL;
    prev = NULL;
    lastVal = 0;
}

bool Node::contains(Vec2f p)
{
    return (p-pos).length() <= 5;
}


void Node::setName(std::string n)
{
    name = n;
    nameSize = ResourceManager::getInstance().getTextureFont()->measureString(name);
}


XmlTree Node::getXml()
{
    XmlTree nodeXml("Node", "");
    nodeXml.setAttribute("globalID", id);
    nodeXml.setAttribute("index", index);
    nodeXml.setAttribute("lastVal", lastVal);
    nodeXml.setAttribute("pos", pos);
    nodeXml.setAttribute("name", name);
    
    return nodeXml;
}


