//
//  Component.h
//  Controlease
//
//  Created by Gal Sasson on 2/22/14.
//
//

#ifndef __Controlease__CanvasComponent__
#define __Controlease__CanvasComponent__

#include <vector>
#include <boost/algorithm/string.hpp>

#include "cinder/app/App.h"
#include "cinder/Xml.h"

#include "Controlease.h"
#include "KeyboardListener.h"
#include "MouseListener.h"
#include "ConnectionResult.h"
//#include "Node.h"
#include "InputNode.h"
#include "OutputNode.h"

class Canvas;
class Node;

using namespace ci;
using namespace std;

class CanvasComponent : public MouseListener
{
public:
    CanvasComponent(Canvas *c);
    virtual ~CanvasComponent();
    
    virtual void initNew(Vec2f p);
    virtual void initFromXml(const XmlTree& xml, bool createNodes = true);
    virtual XmlTree getXml();
        
    void setType(ComponentType t);
    void setSize(Vec2f size);
    virtual void setName(std::string n);
    
    virtual void update() {};
    virtual void draw() {};
    virtual void drawOutline();
    virtual void translate(const Vec2f& offset);
    virtual Rectf getBounds();
    virtual void applyBorders();
    virtual Vec2f getCanvasPos();
    virtual bool isDragPoint(const cease::MouseEvent& event) {return false;};
    virtual bool isHotspot(const cease::MouseEvent& event) {return false;};

    virtual ConnectionResult getConnectionStart(const cease::MouseEvent& event);
    virtual ConnectionResult getConnectionEnd(const cease::MouseEvent& event);
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
    virtual Node* getNodeBelow(const cease::MouseEvent& event);
    virtual bool contains(const Vec2f& canvasPoint);

    // set component size to match content
    // (number of inputs, outputs, plus sign, name)
    virtual void pack(float minX = 0, float minY = 0);
    
    static std::string getComponentTypeString(ComponentType t);
    static std::string getJSComponentTypeString(std::string scriptFile);

protected:
    virtual Vec2f toLocal(const Vec2f& p);
    virtual Vec2f toCanvas(const Vec2f& p);
    
    Canvas *canvas;
    Vec2f originalSize; // size as set in setSize
    Rectf canvasRect;   // component rect in canvas space
    Rectf localRect;    // component rect in local space
    Rectf titleRect;    // the title rect of the component
    ComponentType type;
    std::string name;
    cinder::Vec2f nameSize;
    
    // component nodes
    InputNode* addNewInputNode();
    OutputNode* addNewOutputNode();
    InputNode* addInputNodeFromXml(const XmlTree& xml);
    OutputNode* addOutputNodeFromXml(const XmlTree& xml);
    Vec2f nextOutputPos;
    Vec2f nextInputPos;
    vector<InputNode*> inputNodes;
    vector<OutputNode*> outputNodes;
    Rectf inputPlusRect;
    Rectf outputPlusRect;
    bool showInputPlus;
    bool showOutputPlus;
};


#endif /* defined(__Controlease__CanvasComponent__) */
