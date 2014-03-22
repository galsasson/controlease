//
//  Component.h
//  Controlease
//
//  Created by Gal Sasson on 2/22/14.
//
//

#ifndef __Controlease__CanvasComponent__
#define __Controlease__CanvasComponent__

#include "KeyboardListener.h"
#include "MouseListener.h"
#include "ConnectionResult.h"
#include "Node.h"
#include "InputNode.h"
#include "OutputNode.h"
#include <vector>

using namespace std;

class CanvasComponent : public MouseListener
{
public:
    virtual ~CanvasComponent() {};
    virtual void update() {};
    virtual void draw() {};
    virtual void drawOutline() {};
    virtual void translate(Vec2f offset) {};
    virtual Rectf getBounds() {return Rectf(0, 0, 0, 0);};
    virtual Vec2f getCanvasPos() {return Vec2f(0, 0);};
    virtual float getValue(int i) {return 0;};
    virtual void setValue(int i, float v) {};
    virtual bool isDragPoint(cease::MouseEvent event) {return false;};
    virtual bool isHotspot(cease::MouseEvent event) {return false;};
    virtual ConnectionResult* getConnectionStart( cease::MouseEvent event) { return NULL; };
    virtual ConnectionResult* getConnectionEnd( cease::MouseEvent event) { return NULL; };
    virtual vector<Node*> getInputNodes() { return vector<Node*>(); };
    virtual vector<Node*> getOutputNodes() { return vector<Node*>(); };
    virtual KeyboardListener* getCurrentKeyboardListener() {return NULL;};
    virtual Node* getNodeBelow(cease::MouseEvent event);
    
protected:
    vector<InputNode*> inputNodes;
    vector<OutputNode*> outputNodes;
};


#endif /* defined(__Controlease__CanvasComponent__) */
