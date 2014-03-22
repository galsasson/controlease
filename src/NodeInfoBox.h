//
//  NodeInfoBox.h
//  Controlease
//
//  Created by Gal Sasson on 3/22/14.
//
//

#ifndef __Controlease__NodeInfoBox__
#define __Controlease__NodeInfoBox__

#include <iostream>
#include "cinder/gl/gl.h"
#include "cinder/app/AppNative.h"
#include "ResourceManager.h"
#include "Node.h"

using namespace ci;
using namespace ci::gl;

class NodeInfoBox
{
public:
    NodeInfoBox(Node* n);
    void setNode(Node* n);
    void update();
    void draw();
    
    Node* node;
private:
    void setSize(float w, float h);
    
    Rectf canvasRect;
    Rectf localRect;
    Rectf nameRect;
    Rectf valueRect;
    
    std::string strVal;
};

#endif /* defined(__Controlease__NodeInfoBox__) */
