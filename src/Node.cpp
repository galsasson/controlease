//
//  Node.cpp
//  Controlease
//
//  Created by Gal Sasson on 3/24/14.
//
//

#include "Node.h"

int Node::globalNodeID = 0;

Node::Node(CanvasComponent *comp)
{
    component = comp;
    
    bDrawActive = false;
    bDisplayName = false;
    
    next = NULL;
    prev = NULL;
}

void Node::initNew(int i, Vec2f p)
{
    id = Node::globalNodeID++;

    index = i;
    pos = p;
    
    lastVal = 0;
}

void Node::initFromXml(const cinder::XmlTree& xml)
{
    id = xml.getAttributeValue<int>("globalID");
    // when loading a patch, update globalID so future
    // nodes will be created with a unique id
    if (id >= Node::globalNodeID) {
        Node::globalNodeID = id+1;
    }
    
    index = xml.getAttributeValue<int>("index");
    pos = Vec2f(xml.getAttributeValue<float>("pos.x"), xml.getAttributeValue<float>("pos.y"));
    setName(xml.getAttributeValue<std::string>("name"));
    lastVal = xml.getAttributeValue<float>("lastVal");
    bDisplayName = xml.getAttributeValue<bool>("bDisplayName");
}

XmlTree Node::getXml()
{
    XmlTree nodeXml("Node", "");
    nodeXml.setAttribute("globalID", id);
    nodeXml.setAttribute("index", index);
    nodeXml.setAttribute("lastVal", lastVal);
    nodeXml.setAttribute("pos.x", pos.x);
    nodeXml.setAttribute("pos.y", pos.y);
    nodeXml.setAttribute("name", name);
    nodeXml.setAttribute("bDisplayName", bDisplayName);
    
    return nodeXml;
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


