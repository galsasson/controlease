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
    InputNode(CanvasComponent *comp);
    ~InputNode();
    
    void initNew(int i, Vec2f p);
    void initFromXml(const XmlTree &xml);
    XmlTree getXml();
    
    void draw();
    
    void updateVal(float val, bool force = false);
    void connect(Node *node);
    void disconnect(Node *node);
    bool isConnected();
    
    void setOriginalVal(float val);
    
    Vec2f getCanvasPos();
    
private:
    float originalVal;  // this value will be set as the value when the node is disconnected
};


#endif /* defined(__Controlease__InputNode__) */
