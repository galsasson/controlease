//
//  Wire.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/22/14.
//
//

#include "Wire.h"

Wire::Wire()
{
    inode = NULL;
    onode = NULL;
}

Wire::~Wire()
{
    if (inode) {
        disconnectInput();
    }
    
    if (onode) {
        disconnectOutput();
    }
}

void Wire::update()
{
    
}

void Wire::draw()
{
    gl::color(1, 1, 1);
    
    gl::lineWidth(2);
    if (inode && onode) {
        gl::drawLine(inode->getCanvasPos(), onode->getCanvasPos());
    }
    else if (inode) {
        gl::drawLine(inode->getCanvasPos(), currentEnd);
    }
    else if (onode) {
        gl::drawLine(currentEnd, onode->getCanvasPos());
    }
    gl::lineWidth(1);
}

void Wire::setEnd(Vec2f p)
{
    currentEnd = p;
}

bool Wire::addConnectable(ConnectionResult con)
{
    if (con.type == TYPE_INPUT) {
        if (inode != NULL) {
            return false;
        }
        
        inode = (InputNode*)con.node;
        currentEnd = inode->getCanvasPos();
    }
    else if (con.type == TYPE_OUTPUT) {
        if (onode != NULL) {
            return false;
        }
        
        onode = (OutputNode*)con.node;
        currentEnd = onode->getCanvasPos();
    }
    
    if (onode != NULL && inode != NULL) {
        // we have input and output, create connection
        onode->connect(inode);
        inode->connect(onode);
        return true;
    }
    
    return false;
}

bool Wire::haveNode(Node *node)
{
    return node == inode || node == onode;
}

void Wire::disconnectInput()
{
    if (inode) {
        currentEnd = inode->getCanvasPos();
        inode->disconnect(onode);
    }
    if (onode) {
        onode->disconnect(inode);
    }
    inode = NULL;
}

void Wire::disconnectOutput()
{
    if (onode) {
        currentEnd = onode->getCanvasPos();
        onode->disconnect(inode);
    }
    if (inode) {
        inode->disconnect(onode);
    }
    onode = NULL;
}

XmlTree Wire::getXml()
{
    XmlTree xml("Wire", "");
    
    if (inode && onode)
    {
        xml.setAttribute("inputID", inode->getId());
        xml.setAttribute("outputID", onode->getId());
    }
    
    return xml;
}

