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
#include "ResourceManager.h"
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
    
    void updateVal(float val, bool force = false);
    void connect(Node *node);
    void disconnect(Node *node);
    bool isConnected();
    
    Vec2f getCanvasPos();
    
private:
    int index;
    bool bDrawActive;
    
    // the component
    CanvasComponent *component;
};


#endif /* defined(__Controlease__InputNode__) */
