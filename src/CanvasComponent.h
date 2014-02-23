//
//  Component.h
//  Controlease
//
//  Created by Gal Sasson on 2/22/14.
//
//

#ifndef __Controlease__Component__
#define __Controlease__Component__

#include "MouseListener.h"
#include "ConnectionResult.h"
#include "Node.h"
#include <vector>

using namespace std;

class CanvasComponent : public MouseListener
{
public:
    virtual ~CanvasComponent() {};
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void drawOutline() = 0;
    virtual Vec2f getCanvasPos() = 0;
    virtual float getValue(int i) = 0;
    virtual void setValue(int i, float v) = 0;
    virtual ConnectionResult* getConnectionStart( cease::MouseEvent event) = 0;
    virtual ConnectionResult* getConnectionEnd( cease::MouseEvent event) = 0;
    virtual vector<Node*> getInputNodes() = 0;
    virtual vector<Node*> getOutputNodes() = 0;
};


#endif /* defined(__Controlease__Component__) */
