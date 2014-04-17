//
//  OutputNode.h
//  Controlease
//
//  Created by Gal Sasson on 2/22/14.
//
//

#ifndef __Controlease__OutputNode__
#define __Controlease__OutputNode__

#include <iostream>
#include <sstream>
#include "cinder/app/App.h"
//#include "CanvasComponent.h"
#include "Node.h"

using namespace ci;
using namespace ci::gl;

class CanvasComponent;

class OutputNode : public Node
{
public:
    OutputNode(CanvasComponent *comp);
    ~OutputNode();
    
    void initNew(int i, Vec2f p);
    
    void draw();
    
    void updateVal(float val, bool force = false);
    void connect(Node *node);
    void disconnect(Node *node);
    bool isConnected();
    std::string getEndpointName();
    
    float getLastVal() { return lastVal; }
    Vec2f getCanvasPos();
};

#endif /* defined(__Controlease__OutputNode__) */
