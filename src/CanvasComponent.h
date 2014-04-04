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

class Canvas;

using namespace std;

typedef enum _ComponentType {
    COMPONENT_TYPE_PROGRAM,
    COMPONENT_TYPE_NUMBER,
    COMPONENT_TYPE_SPLIT,
    COMPONENT_TYPE_OSCILLATOR,
    COMPONENT_TYPE_EXP,
    COMPONENT_TYPE_JS,
    COMPONENT_TYPE_OSCCONTROLLER,
} ComponentType;

class CanvasComponent : public MouseListener
{
public:
    CanvasComponent(Canvas *c, ComponentType type);
    virtual ~CanvasComponent();
    virtual void update() {};
    virtual void draw() {};
    virtual void drawOutline() {};
    virtual void translate(Vec2f offset);
    virtual Rectf getBounds();
    virtual void applyBorders();
    virtual Vec2f getCanvasPos();
    virtual bool isDragPoint(cease::MouseEvent event) {return false;};
    virtual bool isHotspot(cease::MouseEvent event) {return false;};

    virtual ConnectionResult* getConnectionStart( cease::MouseEvent event);
    virtual ConnectionResult* getConnectionEnd( cease::MouseEvent event);
    virtual vector<Node*> getInputNodes();
    virtual vector<Node*> getOutputNodes();
    virtual Node* getNodeWithID(int id);
    
    // callbacks from nodes
    virtual float getValue(int i) {return 0;};
    virtual void setValue(int i, float v) {};
    virtual void outputConnected(int i) {};
    virtual void outputDisconnected(int i) {};
    virtual void inputConnected(int i) {};
    virtual void inputDisconnected(int i) {};

    virtual KeyboardListener* getCurrentKeyboardListener() {return NULL;};
    virtual Node* getNodeBelow(cease::MouseEvent event);
    virtual bool contains(Vec2f canvasPoint);
    
    virtual void setName(std::string n);
    static std::string getComponentTypeString(ComponentType t);
    static std::string getJSComponentTypeString(std::string scriptFile);

    ComponentType type;
    std::string name;
    cinder::Vec2f nameSize;
protected:
    virtual Vec2f toLocal(Vec2f p);
    virtual Vec2f toCanvas(Vec2f p);
    
    Canvas *canvas;
    Rectf canvasRect;
    
    vector<InputNode*> inputNodes;
    vector<OutputNode*> outputNodes;
};


#endif /* defined(__Controlease__CanvasComponent__) */
