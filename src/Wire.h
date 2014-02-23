//
//  Wire.h
//  Controlease
//
//  Created by Gal Sasson on 2/22/14.
//
//

#ifndef __Controlease__Wire__
#define __Controlease__Wire__

#include <iostream>
#include <vector>
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

#include "Controlease.h"
#include "ConnectionResult.h"
#include "InputNode.h"
#include "OutputNode.h"

using namespace ci;
using namespace ci::gl;

class Wire
{
public:
    Wire();
    ~Wire();
    
    void update();
    void draw();
    
    void addPoint(Vec2f p);
    void setEnd(Vec2f p);
    
    bool addConnectable(ConnectionResult* con);
    bool haveNode(Node *node);
    
    void disconnectInput();
    void disconnectOutput();
    
private:
    Vec2f currentEnd;
    
    InputNode *inode;
    OutputNode *onode;
};

#endif /* defined(__Controlease__Wire__) */
