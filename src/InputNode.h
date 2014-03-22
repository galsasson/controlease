//
//  InputNode.h
//  Controlease
//
//  Created by Gal Sasson on 2/22/14.
//
//

#ifndef __Controlease__InputNode__
#define __Controlease__InputNode__

#include <iostream>
#include "cinder/app/App.h"
#include "Node.h"
//#include "CanvasComponent.h"

using namespace ci;
using namespace ci::gl;

class CanvasComponent;

class InputNode : public Node
{
public:
    InputNode(int i, CanvasComponent *val, Vec2f p);
    ~InputNode();
    
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


#endif /* defined(__Controlease__InputNode__) */
