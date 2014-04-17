//
//  Node.cpp
//  Controlease
//
//  Created by Gal Sasson on 3/24/14.
//
//

#include "Node.h"

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
    index = i;
    pos = p;
    
    lastVal = 0;
}

void Node::initFromXml(const cinder::XmlTree& xml)
{
    id = xml.getAttributeValue<std::string>("id");
    index = xml.getAttributeValue<int>("index");
    pos = Vec2f(xml.getAttributeValue<float>("pos.x"), xml.getAttributeValue<float>("pos.y"));
    setName(xml.getAttributeValue<std::string>("name"));
    lastVal = xml.getAttributeValue<float>("lastVal");
    bDisplayName = xml.getAttributeValue<bool>("bDisplayName");
}

XmlTree Node::getXml()
{
    XmlTree nodeXml("Node", "");
    nodeXml.setAttribute("id", id);
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


