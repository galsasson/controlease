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
    JSComponent(Vec2f p, fs::path script);
    ~JSComponent();
    
    void initNodes(int nIns, int nOuts);
    void initGUI(Vec2f size);
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
    
    bool contains(Vec2f p);
    Vec2f getCanvasPos();
    
    float getValue(int i);
    void setValue(int i, float v);
    
private:
    void applyBorders();
    void resizeComponent();
    
    Vec2f getLocalCoords(Vec2f p);
    Vec2f getCanvasCoords(Vec2f p);
    
    Rectf canvasRect;
    Rectf localRect;
    Rectf titleRect;
    Rectf jsRect;
    
    // color of js drawings
    Color jsColor;
    Vec3f jsColorVec;
    Perlin perlin;
    
    // component dragging
    Vec2f compDragAnchor;
    bool isDragging;
    
    Vec2f nextOutputPos;
    Vec2f nextInputPos;
//    vector<OutputNode*> outputNodes;
//    vector<InputNode*> inputNodes;
    
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
    // draw string
    static void v8SetBrightnessCB(const FunctionCallbackInfo<v8::Value>& args);
    static void v8SetBWCB(const FunctionCallbackInfo<v8::Value>& args);
    static void v8SetHueCB(const FunctionCallbackInfo<v8::Value>& args);
    static void v8NoiseCB(const FunctionCallbackInfo<v8::Value>& args);
    
    
    void v8Init(const FunctionCallbackInfo<v8::Value>& args); // (name, inputs, outputs)
    void v8SetGuiSize(const FunctionCallbackInfo<v8::Value>& args); // (width, height)
    void v8DrawEllipse(const FunctionCallbackInfo<v8::Value>& args); // (cx, cy, w, h)
    void v8DrawLine(const FunctionCallbackInfo<v8::Value>& args); // (sx, sy, tx, ty)
    void v8DrawRect(const FunctionCallbackInfo<v8::Value>& args); // (ulx, uly, width, height)
    void v8SetBrightness(const FunctionCallbackInfo<v8::Value>& args); // (0-1)
    void v8SetBW(const FunctionCallbackInfo<v8::Value>& args);   // (0-1)
    void v8SetHue(const FunctionCallbackInfo<v8::Value>& args); // (0-1)
    void v8Noise(const FunctionCallbackInfo<v8::Value>& args);
    
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
