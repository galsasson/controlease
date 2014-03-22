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
    virtual ~CanvasComponent();
    virtual void update() {};
    virtual void draw() {};
    virtual void drawOutline() {};
    virtual void translate(Vec2f offset);
    virtual Rectf getBounds();
    virtual Vec2f getCanvasPos();
    virtual float getValue(int i) {return 0;};
    virtual void setValue(int i, float v) {};
    virtual bool isDragPoint(cease::MouseEvent event) {return false;};
    virtual bool isHotspot(cease::MouseEvent event) {return false;};
    virtual ConnectionResult* getConnectionStart( cease::MouseEvent event);
    virtual ConnectionResult* getConnectionEnd( cease::MouseEvent event);
    virtual vector<Node*> getInputNodes();
    virtual vector<Node*> getOutputNodes();
    virtual KeyboardListener* getCurrentKeyboardListener() {return NULL;};
    virtual Node* getNodeBelow(cease::MouseEvent event);
    virtual bool contains(Vec2f canvasPoint);
    
protected:
    virtual Vec2f toLocal(Vec2f p);
    virtual Vec2f toCanvas(Vec2f p);
    
    Rectf canvasRect;
    
    vector<InputNode*> inputNodes;
    vector<OutputNode*> outputNodes;
};


#endif /* defined(__Controlease__CanvasComponent__) */
