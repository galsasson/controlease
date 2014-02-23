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
#include "CanvasComponent.h"
#include "Node.h"

using namespace ci;
using namespace ci::gl;

class OutputNode : public Node
{
public:
    OutputNode(int i, CanvasComponent *comp, Vec2f p);
    
    void draw();
    bool contains(Vec2f p);
    
    void updateVal(float val);
    void connect(Node *node);
    void disconnect(Node *node);
    bool isConnected();
    
    Vec2f getCanvasPos();
    
private:
    Vec2f pos;
    int index;
    
    // the component
    CanvasComponent *component;
};

#endif /* defined(__Controlease__OutputNode__) */
