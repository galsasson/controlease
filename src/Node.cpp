//
//  Node.cpp
//  Controlease
//
//  Created by Gal Sasson on 3/24/14.
//
//

#include "Node.h"

static int nodeID = 0;

Node::Node()
{
    id = nodeID++;
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

