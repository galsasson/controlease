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
#include "cinder/app/App.h"
//#include "CanvasComponent.h"
#include "Node.h"

using namespace ci;
using namespace ci::gl;

class CanvasComponent;

class OutputNode : public Node
{
public:
    OutputNode(int i, CanvasComponent *comp, Vec2f p);
    ~OutputNode();
    
    void draw();
    
    void updateVal(float val, bool force = false);
    void connect(Node *node);
    void disconnect(Node *node);
    bool isConnected();
    std::string getEndpointName();
    
    float getLastVal() { return lastVal; }
    
    bool bDisplayName;
    Vec2f getCanvasPos();

private:
    int index;
    bool bFillEllipse;
    
    // the component
    CanvasComponent *component;
};

#endif /* defined(__Controlease__OutputNode__) */
