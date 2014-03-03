//
//  Exp.h
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#ifndef __Controlease__Exp__
#define __Controlease__Exp__

#include <iostream>
#include <sstream>
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rect.h"
#include <boost/container/vector.hpp>
#include "v8.h"


#include "Controlease.h"
#include "ResourceManager.h"
#include "MouseListener.h"
#include "InputNode.h"
#include "OutputNode.h"
#include "TextInput.h"
#include "ConnectionResult.h"
#include "CanvasComponent.h"


using namespace ci;
using namespace ci::app;
using namespace std;
using namespace v8;

class Exp : public CanvasComponent
{
public:
    Exp(Vec2f p, Vec2f s);
    ~Exp();
    
    void initInterface(Vec2f size);
    void update();
    void draw();
    void drawOutline();
    
    void translate(Vec2f offset);
    Rectf getBounds();
    
    void mouseDown( cease::MouseEvent event );
	void mouseUp( cease::MouseEvent event);
	void mouseWheel( cease::MouseEvent event );
	void mouseMove( cease::MouseEvent event );
	void mouseDrag( cease::MouseEvent event );
    bool isDragPoint( cease::MouseEvent event);
    bool isHotspot( cease::MouseEvent event);
    ConnectionResult* getConnectionStart( cease::MouseEvent event);
    ConnectionResult* getConnectionEnd( cease::MouseEvent event);
    vector<Node*> getInputNodes();
    vector<Node*> getOutputNodes();
    
    KeyboardListener* getCurrentKeyboardListener();
    void inputEnterPressed();
    
    bool contains(Vec2f p);
    Vec2f getCanvasPos();
    
    float getValue(int i);
    void setValue(int i, float v);
    
private:
    void applyBorders();
    void resizeComponent();
    
    TextInput* codeInput;
    bool isEditing;
    Rectf textEditRect;
    
    Vec2f getLocalCoords(Vec2f p);
    Vec2f getCanvasCoords(Vec2f p);
    
    Rectf canvasRect;
    Rectf rect;
    Rectf titleRect;
    Rectf inPlusRect;
    Rectf outPlusRect;
    Rectf dragRect;
    Rectf valRect;
    
    // component dragging
    Vec2f compDragAnchor;
    
    Vec2f nextOutputPos;
    Vec2f nextInputPos;
    vector<OutputNode*> outputNodes;
    vector<InputNode*> inputNodes;
    
    vector<float> ivals;
    
    // V8
    static void v8Getter(uint32_t index, const PropertyCallbackInfo<Value>& info);
    static void v8Setter(uint32_t index, Local<Value> value, const PropertyCallbackInfo<Value>& info);
    void compileAndRun(std::string code);
    void runCompiledScript();
    Persistent<Context> pContext;
    Persistent<Function> pFunction;
    
};

#endif /* defined(__Controlease__Exp__) */
