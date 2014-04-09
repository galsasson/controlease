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
    Exp(Canvas *c);
    ~Exp();
    
    void initNew(Vec2f pos);
    void initFromXml(const XmlTree& xml);
    
    XmlTree getXml();

    void inputEnterPressed();

    void update();
    void draw();
    void drawOutline();
    
    void mouseDown(const cease::MouseEvent& event);
	void mouseDrag(const cease::MouseEvent& event);
    bool isDragPoint(const cease::MouseEvent& event);
    bool isHotspot(const cease::MouseEvent& event);
    KeyboardListener* getCurrentKeyboardListener();
    
    void resizeComponent();
    
    float getValue(int i);
    void setValue(int i, float v);
    
private:
    void prepareJSScript();
    TextInput* codeInput;
    bool isEditing;
    Rectf textEditRect;
    
    // component dragging
    Vec2f compDragAnchor;
    
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
