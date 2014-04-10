//
//  Component.h
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#ifndef __Controlease__JSComponent__
#define __Controlease__JSComponent__

#include <iostream>
#include <sstream>
#include <fstream>
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rect.h"
#include <boost/container/vector.hpp>
#include "cinder/Perlin.h"
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

class JSComponent : public CanvasComponent
{
public:
    JSComponent(Canvas *c);
    ~JSComponent();
    
    void initNew(Vec2f pos, fs::path script);
    void initFromXml(const XmlTree& xml);
    XmlTree getXml();
    
    void initNodes(int nIns, int nOuts);
    void initGUI(Vec2f size);
    void update();
    void draw();
    
    void mouseDown(const cease::MouseEvent& event);
	void mouseUp(const cease::MouseEvent& event);
	void mouseDrag(const cease::MouseEvent& event);
    bool isDragPoint(const cease::MouseEvent& event);
    bool isHotspot(const cease::MouseEvent& event);
    
    float getValue(int i);
    void setValue(int i, float v);
    
private:
    void resizeComponent();
    
    Rectf originRect;
    Rectf jsRect;
    
    // color of js drawings
    Color jsColor;
    Vec3f jsColorVec;
    Perlin perlin;
    
    // component dragging
    Vec2f compDragAnchor;
    bool isDragging;
    
    vector<float> ivals;
    
    /***********************/
    /* V8 (javascript) API */
    /***********************/
    fs::path jsScript;
    std::string compName;
    void initComponent();
    
    // get in value
    static void v8InGetter(uint32_t index, const PropertyCallbackInfo<Value>& info);
    // get out value
    static void v8OutGetter(uint32_t index, const PropertyCallbackInfo<Value>& info);
    // set out value
    static void v8OutSetter(uint32_t index, Local<Value> value, const PropertyCallbackInfo<Value>& info);
    // 'map' function
    static void v8Map(const FunctionCallbackInfo<v8::Value>& args);
    static void v8InitCB(const FunctionCallbackInfo<v8::Value>& args);
    static void v8SetGuiSizeCB(const FunctionCallbackInfo<v8::Value>& args);
    static void v8DrawEllipseCB(const FunctionCallbackInfo<v8::Value>& args);
    static void v8DrawLineCB(const FunctionCallbackInfo<v8::Value>& args);
    static void v8DrawRectCB(const FunctionCallbackInfo<v8::Value>& args);
    static void v8DrawStringCB(const FunctionCallbackInfo<v8::Value>& args);
    static void v8SetBrightnessCB(const FunctionCallbackInfo<v8::Value>& args);
    static void v8SetBWCB(const FunctionCallbackInfo<v8::Value>& args);
    static void v8SetHueCB(const FunctionCallbackInfo<v8::Value>& args);
    static void v8NoiseCB(const FunctionCallbackInfo<v8::Value>& args);
    
    // moving around
    static void v8SetOffsetCB(const FunctionCallbackInfo<v8::Value>& args);
    static void v8GetPositionCB(const FunctionCallbackInfo<v8::Value>& args);
    // getting free inputs
    static void v8GetCanvasInputsCB(const FunctionCallbackInfo<v8::Value>& args);
    static void v8ConnectOutputCB(const FunctionCallbackInfo<v8::Value>& args);
    static void v8DisconnectOutputCB(const FunctionCallbackInfo<v8::Value>& args);

    static void v8LogCB(const FunctionCallbackInfo<v8::Value>& args);

    
    
    void v8Init(const FunctionCallbackInfo<v8::Value>& args); // (name, inputs, outputs)
    void v8SetGuiSize(const FunctionCallbackInfo<v8::Value>& args); // (width, height)
    void v8DrawEllipse(const FunctionCallbackInfo<v8::Value>& args); // (cx, cy, w, h)
    void v8DrawLine(const FunctionCallbackInfo<v8::Value>& args); // (sx, sy, tx, ty)
    void v8DrawRect(const FunctionCallbackInfo<v8::Value>& args); // (ulx, uly, width, height)
    void v8DrawString(const FunctionCallbackInfo<v8::Value>& args); // (str, x, y)
    
    void v8SetBrightness(const FunctionCallbackInfo<v8::Value>& args); // (0-1)
    void v8SetBW(const FunctionCallbackInfo<v8::Value>& args);   // (0-1)
    void v8SetHue(const FunctionCallbackInfo<v8::Value>& args); // (0-1)
    void v8Noise(const FunctionCallbackInfo<v8::Value>& args);
    // moving around
    void v8SetOffset(const FunctionCallbackInfo<v8::Value>& args);
    void v8GetPosition(const FunctionCallbackInfo<v8::Value>& args);
    // getting free inputs
    void v8GetCanvasInputs(const FunctionCallbackInfo<v8::Value>& args);
    void v8ConnectOutput(const FunctionCallbackInfo<v8::Value>& args);
    void v8DisconnectOutput(const FunctionCallbackInfo<v8::Value>& args);
    
    void v8Log(const FunctionCallbackInfo<v8::Value>& args);
    
    bool getFunction(Handle<Context> &context, std::string name, Persistent<Function> &func);
    bool callV8Function(Persistent<Function> &func);
    bool callV8MouseFunction(Persistent<Function> &func, float x, float y);
    
    void compileAndRun(std::string code);
    void scriptRunMouseDown();
    void scriptRunMouseUp();
    Persistent<Context> pContext;
    Persistent<Function> pSetupFunc;
    Persistent<Function> pUpdateFunc;
    Persistent<Function> pDrawFunc;
    Persistent<Function> pMouseDownFunc;
    Persistent<Function> pMouseUpFunc;
    Persistent<Function> pMouseDragFunc;
    
};

#endif /* defined(__Controlease__JSComponent__) */
