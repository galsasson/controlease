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
    KeyboardListener* getCurrentKeyboardListener();
    void inputEnterPressed();
    
    float getValue(int i);
    void setValue(int i, float v);
    
private:
    void resizeComponent();
    
    TextInput* codeInput;
    bool isEditing;
    Rectf textEditRect;
    
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
//    vector<OutputNode*> outputNodes;
//    vector<InputNode*> inputNodes;
    
    vector<float> ivals;
    
    // V8
    static void v8InGetter(uint32_t index, const PropertyCallbackInfo<Value>& info);
    static void v8OutGetter(uint32_t index, const PropertyCallbackInfo<Value>& info);
    static void v8OutSetter(uint32_t index, Local<Value> value, const PropertyCallbackInfo<Value>& info);
    static void v8Map(const FunctionCallbackInfo<v8::Value>& args);
    
    void compileAndRun(std::string code);
    void runCompiledScript();
    Persistent<Context> pContext;
    Persistent<Function> pFunction;
    
};

#endif /* defined(__Controlease__Exp__) */
