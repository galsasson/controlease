//
//  Node.h
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#ifndef Controlease_Node_h
#define Controlease_Node_h

#include "cinder/gl/gl.h"
#include "cinder/Rect.h"
#include "cinder/app/App.h"
#include "cinder/Xml.h"
#include "Controlease.h"
#include "ResourceManager.h"
//#include "CanvasComponent.h"

class CanvasComponent;

class Node
{
public:
    Node(CanvasComponent *comp);
    ~Node() {};
    
    virtual void initNew(int i, Vec2f p);
    virtual void initFromXml(const XmlTree& xml);
    virtual XmlTree getXml();
        
    virtual void updateVal(float val, bool force = false) = 0;
    virtual void connect(Node *node) = 0;
    virtual void disconnect(Node *node) = 0;
    virtual bool isConnected() = 0;
    virtual cinder::Vec2f getCanvasPos() = 0;
    
    virtual bool contains(cinder::Vec2f p);
    virtual void setName(std::string name);
    virtual std::string& getName() { return name; }
    virtual float getLastVal() { return lastVal; }
    virtual Vec2f& getPosition() { return pos; }
    virtual void offsetPosition(const Vec2f& offset) {pos += offset; }
    virtual std::string getId() { return id; }

    bool bDisplayName;      // display the name as well

protected:
    std::string id;
    Node *prev;
    Node *next;
    
    // the holding component
    CanvasComponent *component;
    // index in the component
    int index;
    
    float lastVal;
    cinder::Vec2f pos;
    std::string name;       // node name
    Vec2f nameSize;         // dimensions of name
    bool bDrawActive;       // draw the node as sending
};

#endif
