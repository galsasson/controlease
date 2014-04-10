//
//  JSComponent.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "JSComponent.h"
#include "Canvas.h"

JSComponent::JSComponent(Canvas *c) : CanvasComponent(c)
{
    type = ComponentType::COMPONENT_TYPE_JS;
    isDragging = false;

    jsRect = Rectf(10, 26, 0, 0);
    jsColor = Color(0, 0, 0);
    jsColorVec = Vec3f(0, 0, 0);
}

JSComponent::~JSComponent()
{
    pSetupFunc.Reset();
    pUpdateFunc.Reset();
    pDrawFunc.Reset();
    pMouseDownFunc.Reset();
    pMouseUpFunc.Reset();
    pContext.Reset();
}

void JSComponent::initNew(Vec2f pos, fs::path script)
{
    CanvasComponent::initNew(pos);
    setSize(Vec2f(10, 0));
    setName(script.filename().replace_extension("").string());
    jsScript = script;

    originRect = canvasRect;
    
    initComponent();
}

void JSComponent::initFromXml(const XmlTree& xml)
{
    CanvasComponent::initFromXml(xml);
    stringstream compPath;
    compPath << getJSComponentsPath();
    compPath << xml.getAttributeValue<std::string>("jsScript");
    jsScript = fs::path(compPath.str());
    
    initComponent();
    setInternalState(xml.getAttributeValue<std::string>("stateJson"));
}

XmlTree JSComponent::getXml()
{
    XmlTree xml = CanvasComponent::getXml();
    
    xml.setAttribute("jsScript", jsScript.filename().string());
    xml.setAttribute("stateJson", getInternalState());
    
    return xml;
}

void JSComponent::initNodes(int nIns, int nOuts)
{
    for (int i=inputNodes.size(); i<nIns; i++)
    {
        addNewInputNode();
    }
    for (int i=outputNodes.size(); i<nOuts; i++)
    {
        addNewOutputNode();
    }

    resizeComponent();
}

void JSComponent::initGUI(Vec2f size)
{
    jsRect.x2 = jsRect.x1 + size.x;
    jsRect.y2 = jsRect.y1 + size.y;
    
    resizeComponent();
}

void JSComponent::update()
{
    // call update function of javascript
    callV8Function(pUpdateFunc);
}

void JSComponent::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(localRect, 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(localRect, 2);
    
    // draw title
    ResourceManager::getInstance().getTextureFont()->drawString(compName, titleRect);
    gl::drawLine(Vec2f(0, titleRect.y2), Vec2f(localRect.getWidth(), titleRect.y2));

    // draw inputs and outputs
    gl::color(0, 0, 0);
    // draw nodes
    for (int i=0; i<inputNodes.size(); i++) {
        inputNodes[i]->draw();
    }
    for (int i=0; i<outputNodes.size(); i++) {
        outputNodes[i]->draw();
    }
    
    // call draw of js component
    gl::pushMatrices();
    gl::translate(jsRect.getUpperLeft() + jsRect.getSize()/2);
    gl::color(jsColor);
    callV8Function(pDrawFunc);
    gl::popMatrices();
    
    gl::popMatrices();

    gl::color(255, 0, 0);
    gl::drawStrokedRect(originRect);
}

void JSComponent::mouseDown(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());

    if (titleRect.contains(local)) {
        // drag the component
        isDragging = true;
        compDragAnchor = event.getPos();
    }
    else if (jsRect.contains(local)) {
        // call mouse down of js component
        callV8MouseFunction(pMouseDownFunc, local.x - jsRect.x1, local.y - jsRect.y1);
    }
}

void JSComponent::mouseDrag(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    if (isDragging)
    {
        originRect += (event.getPos() - compDragAnchor);
        canvasRect += (event.getPos() - compDragAnchor);
        compDragAnchor = event.getPos();
        applyBorders();
    }
    else if (jsRect.contains(local)) {
        // call drag on js component
        callV8MouseFunction(pMouseDragFunc, local.x - jsRect.x1, local.y - jsRect.y1);
    }
}

void JSComponent::mouseUp(const cease::MouseEvent& event)
{
    isDragging = false;
    
    // call mouse up of js component
    Vec2f local = toLocal(event.getPos());
    if (jsRect.contains(local)) {
        callV8MouseFunction(pMouseUpFunc, local.x - jsRect.x1, local.y - jsRect.y1);
    }
}

bool JSComponent::isDragPoint(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    
    return titleRect.contains(local) || jsRect.contains(local);
}

bool JSComponent::isHotspot(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    
    return titleRect.contains(local) ||
            jsRect.contains(local);
}

float JSComponent::getValue(int i)
{
    // this returns the input values
    if (i >= inputNodes.size()) {
        return 0;
    }
    
    return inputNodes[i]->getLastVal();
}

void JSComponent::setValue(int i, float v)
{
//    if (i >= ivals.size()) {
//        console() << "warning: index "<<i<<"?? are you crazy? what value are you setting?"<<endl;
//        return;
//    }
//    
//    ivals[i] = v;
}

void JSComponent::resizeComponent()
{
    pack(jsRect.getWidth()+19, jsRect.getHeight()+30);
    
    originRect = canvasRect;
}

void JSComponent::initComponent()
{
    ifstream jsfile(jsScript.string());
    if (!jsfile.is_open()) {
        console() << "error: cannot find file '"<<jsScript.string()<<"'"<<endl;
        return;
    }
    
    stringstream buffer;
    
    // declare the 'state' object that will be saved with the component.
    buffer << "var state = {};"<<endl;
    // declare save and load functions
    buffer << "var saveComp = function() { return JSON.stringify(state); }"<<endl;
    buffer << "var loadComp = function(stateObj) { state = eval('(' + stateObj + ')'); }"<<endl;
    
    // put script contents into the buffer
    buffer << jsfile.rdbuf();
    
    compileAndRun(buffer.str());
}

void JSComponent::v8InGetter(uint32_t index, const PropertyCallbackInfo<v8::Value> &info)
{
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    JSComponent *comp = (JSComponent*)wrap->Value();
    ReturnValue<Value> ret = info.GetReturnValue();
    ret.Set(comp->getValue(index));
}

void JSComponent::v8OutGetter(uint32_t index, const PropertyCallbackInfo<v8::Value> &info)
{
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    JSComponent *comp = (JSComponent*)wrap->Value();
    ReturnValue<Value> ret = info.GetReturnValue();
    if (comp->outputNodes.size() > index) {
        ret.Set(comp->outputNodes[index]->getLastVal());
    }
}

void JSComponent::v8OutSetter(uint32_t index, Local<Value> val, const PropertyCallbackInfo<Value>& info)
{
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    JSComponent *comp = (JSComponent*)wrap->Value();
    if (comp->outputNodes.size() > index) {
        comp->outputNodes[index]->updateVal(val->NumberValue());
    }
    ReturnValue<Value> ret = info.GetReturnValue();
    ret.Set(true);
}

bool JSComponent::getFunction(Handle<Context> &context, std::string name, Persistent<v8::Function> &func)
{
    // find 'update' function
    Handle<String> funcName = String::NewFromUtf8(Isolate::GetCurrent(), name.c_str());
    Handle<Value> funcVal = context->Global()->Get(funcName);
    if (!funcVal->IsFunction()) {
        console() << "could not find '"<<name<<"' function."<<endl;
        return false;
    }
    Handle<Function> localFunction = Handle<Function>::Cast(funcVal);
    func.Reset(Isolate::GetCurrent(), localFunction);

    return true;
}

void JSComponent::compileAndRun(std::string code)
{
    /* compile the script */
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(Isolate::GetCurrent());

    // add callback functions
    Handle<ObjectTemplate> global = ObjectTemplate::New(Isolate::GetCurrent());
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceMap"), FunctionTemplate::New(Isolate::GetCurrent(), v8Map));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceInit"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8InitCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceSetGuiSize"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8SetGuiSizeCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceEllipse"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8DrawEllipseCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceRect"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8DrawRectCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceLine"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8DrawLineCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceDrawString"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8DrawStringCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceBrightness"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8SetBrightnessCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceBW"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8SetBWCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceHue"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8SetHueCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceNoise"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8NoiseCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceSetOffset"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8SetOffsetCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceGetPosition"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8GetPositionCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceGetCanvasInputs"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8GetCanvasInputsCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceConnectOutput"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8ConnectOutputCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceDisconnectOutput"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8DisconnectOutputCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "ceLog"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8LogCB, External::New(Isolate::GetCurrent(), this)));
    
    
    
    // add interceptors to inputs and outputs
    Handle<ObjectTemplate> globalIn = ObjectTemplate::New(Isolate::GetCurrent());
    globalIn->SetInternalFieldCount(1);
    globalIn->SetIndexedPropertyHandler(v8InGetter);
    Handle<ObjectTemplate> globalOut = ObjectTemplate::New(Isolate::GetCurrent());
    globalOut->SetInternalFieldCount(1);
    globalOut->SetIndexedPropertyHandler(v8OutGetter, v8OutSetter);

    // Create a new context.
    Handle<Context> context = Context::New(Isolate::GetCurrent(), NULL, global);
    
    // Enter the context for compiling and running the hello world script.
    Context::Scope context_scope(context);
    
    Local<Object> objIn = globalIn->NewInstance();
    objIn->SetInternalField(0, External::New(Isolate::GetCurrent(), this));
    Local<Object> objOut = globalOut->NewInstance();
    objOut->SetInternalField(0, External::New(Isolate::GetCurrent(), this));
    context->Global()->Set(String::NewFromUtf8(Isolate::GetCurrent(), "inn"), objIn);
    context->Global()->Set(String::NewFromUtf8(Isolate::GetCurrent(), "out"), objOut);

    pContext.Reset(Isolate::GetCurrent(), context);
    
    // Create a string containing the JavaScript source code.
    Handle<String> source = String::NewFromUtf8(Isolate::GetCurrent(), code.c_str());
    
    // Compile the source code.
    Handle<Script> localScript = Script::Compile(source);
    if (localScript.IsEmpty()) {
        return;
    }
    
    localScript->Run();

    // get all functions
    if (!getFunction(context, "setup", pSetupFunc)) {
        return;
    }
    if (!getFunction(context, "update", pUpdateFunc)) {
        return;
    }
    
    // not mandatory
    getFunction(context, "draw", pDrawFunc);
    getFunction(context, "mousedown", pMouseDownFunc);
    getFunction(context, "mouseup", pMouseUpFunc);
    getFunction(context, "mousedrag", pMouseDragFunc);
    getFunction(context, "saveComp", pSaveFunc);
    getFunction(context, "loadComp", pLoadFunc);
    
    callV8Function(pSetupFunc);
}

bool JSComponent::callV8Function(Persistent<v8::Function> &func)
{
    if (func.IsEmpty()) {
        return false;
    }
    
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(Isolate::GetCurrent());
    
    Local<Context> context = Local<Context>::New(Isolate::GetCurrent(), pContext);
    
    Context::Scope context_scope(context);
    
    Local<Function> localFunction = Local<Function>::New(Isolate::GetCurrent(), func);
    localFunction->Call(context->Global(), 0, NULL);

    return true;
}

bool JSComponent::callV8MouseFunction(Persistent<v8::Function> &func, float x, float y)
{
    if (func.IsEmpty()) {
        return false;
    }
    
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(Isolate::GetCurrent());
    
    Local<Context> context = Local<Context>::New(Isolate::GetCurrent(), pContext);
    
    Context::Scope context_scope(context);
    
    Local<Function> localFunction = Local<Function>::New(Isolate::GetCurrent(), func);
    
    Handle<Value> args[2];
    args[0] = v8::Number::New(Isolate::GetCurrent(), x);
    args[1] = v8::Number::New(Isolate::GetCurrent(), y);
    
    localFunction->Call(context->Global(), 2, args);
    
    return true;
}

std::string JSComponent::getInternalState()
{
    if (pSaveFunc.IsEmpty()) {
        return "";
    }

    // Create a stack-allocated handle scope.
    HandleScope handle_scope(Isolate::GetCurrent());
    
    Local<Context> context = Local<Context>::New(Isolate::GetCurrent(), pContext);
    
    Context::Scope context_scope(context);
    
    Local<Function> localFunction = Local<Function>::New(Isolate::GetCurrent(), pSaveFunc);
    
    Handle<Value> result = localFunction->Call(context->Global(), 0, NULL);
    
    v8::String::Utf8Value utfString(result->ToString());
    return std::string(*utfString);
}

void JSComponent::setInternalState(std::string stateJSON)
{
    if (pLoadFunc.IsEmpty()) {
        return;
    }
    
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(Isolate::GetCurrent());
    
    Local<Context> context = Local<Context>::New(Isolate::GetCurrent(), pContext);
    
    Context::Scope context_scope(context);
    
    Local<Function> localFunction = Local<Function>::New(Isolate::GetCurrent(), pLoadFunc);
    Handle<Value> args[1];
    args[0] = v8::String::NewFromUtf8(Isolate::GetCurrent(), stateJSON.data());
    
    localFunction->Call(context->Global(), 1, args);
}

/*****************************************************************************/
/*****************************************************************************/
/* v8 functions                                                              */
/*****************************************************************************/
/*****************************************************************************/

void JSComponent::v8Init(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() < 3) {
        return;
    }
    
    HandleScope scope(args.GetIsolate());
    
    v8::String::Utf8Value v8Str(args[0]->ToString());
    compName = std::string(*v8Str);
    initNodes(args[1]->NumberValue(), args[2]->NumberValue());
}

void JSComponent::v8SetGuiSize(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() < 2) {
        return;
    }

    HandleScope scope(args.GetIsolate());
    
    initGUI(Vec2f(args[0]->NumberValue(), args[1]->NumberValue()));
}

void JSComponent::v8DrawEllipse(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() < 4) {
        return;
    }

    HandleScope scope(args.GetIsolate());
    
    gl::color(jsColor);
    gl::drawStrokedEllipse(Vec2f(args[0]->NumberValue(), args[1]->NumberValue()), args[2]->NumberValue(), args[3]->NumberValue());
}

void JSComponent::v8DrawLine(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() < 4) {
        return;
    }
    
    HandleScope scope(args.GetIsolate());

    gl::color(jsColor);
    gl::drawLine(Vec2f(args[0]->NumberValue(), args[1]->NumberValue()), Vec2f(args[2]->NumberValue(), args[3]->NumberValue()));
}

void JSComponent::v8DrawRect(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() < 4) {
        return;
    }
    
    HandleScope scope(args.GetIsolate());

    float x2 = args[0]->NumberValue() + args[2]->NumberValue();
    float y2 = args[1]->NumberValue() + args[3]->NumberValue();
    
    gl::color(jsColor);
    gl::drawStrokedRect(Rectf(Vec2f(args[0]->NumberValue(), args[1]->NumberValue()), Vec2f(x2, y2)));
}

void JSComponent::v8DrawString(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() != 3) {
        console() << "error: call ceDrawString as follows: (str, x, y)"<<endl;
        return;
    }
    
    HandleScope scope(args.GetIsolate());
    
    v8::String::Utf8Value str(args[0]->ToString());
    float x = args[1]->NumberValue();
    float y = args[2]->NumberValue();

    gl::color(jsColor);
    ResourceManager::getInstance().getTextureFont()->drawString((*str), Vec2f(x, y));
    
    args.GetReturnValue().Set(0);
}


void JSComponent::v8SetBrightness(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() != 1) {
        console() << "warning: wrong args to v8SetBrightness!"<<endl;
    }
    
    HandleScope scope(args.GetIsolate());

    jsColorVec.z = args[0]->NumberValue();
//    console() << "HSV = "<<jsColorVec<<endl;
    jsColor.set(ColorModel::CM_HSV, jsColorVec);
    gl::color(jsColor);
}

void JSComponent::v8SetBW(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() != 1) {
        console() << "warning: wrong args to v8SetHue!"<<endl;
    }
    
    HandleScope scope(args.GetIsolate());

    jsColorVec.y = 1-args[0]->NumberValue();
    jsColor.set(ColorModel::CM_HSV, jsColorVec);
    gl::color(jsColor);
}

void JSComponent::v8SetHue(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() != 1) {
        console() << "warning: wrong args to v8SetHue!"<<endl;
    }
    
    HandleScope scope(args.GetIsolate());

    jsColorVec.x = args[0]->NumberValue();
    jsColor.set(ColorModel::CM_HSV, jsColorVec);
    gl::color(jsColor);
}


void JSComponent::v8Map(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() < 5) {
        return;
    }
    
    HandleScope scope(args.GetIsolate());
    
    // do clamp
    float n = args[0]->NumberValue();
    float nMin = args[1]->NumberValue();
    float nMax = args[2]->NumberValue();
    
    if (n<=nMin) {
        args.GetReturnValue().Set(args[3]->NumberValue());
        return;
    }
    else if (n>=nMax) {
        args.GetReturnValue().Set(args[4]->NumberValue());
        return;
    }
    
    args.GetReturnValue().Set(lmap(args[0]->NumberValue(),
                                   args[1]->NumberValue(),
                                   args[2]->NumberValue(),
                                   args[3]->NumberValue(),
                                   args[4]->NumberValue()));
}

void JSComponent::v8Noise(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length()<1) {
        console() << "warning: no arguments for v8Noise!"<<endl;
        return;
    }
    
    HandleScope scope(args.GetIsolate());
    
    if (args.Length() == 1)
    {
        args.GetReturnValue().Set(perlin.noise(args[0]->NumberValue()));
    }
    else if (args.Length() == 2)
    {
        args.GetReturnValue().Set(perlin.noise(args[0]->NumberValue(), args[1]->NumberValue()));
    }
    else if (args.Length() == 3)
    {
        args.GetReturnValue().Set(perlin.noise(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue()));
    }
    else {
        console() << "warning: v8Noise arguments: (<x>, [y], [z])"<<endl;
        return;
    }
}

void JSComponent::v8SetOffset(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() < 2) {
        console() << "warning: v8Translate args: (x, y)"<<endl;
        return;
    }
    
    canvasRect = originRect+Vec2f(args[0]->NumberValue(), args[1]->NumberValue());
    applyBorders();
}

void JSComponent::v8GetPosition(const FunctionCallbackInfo<v8::Value> &args)
{
    
    Handle<Object> obj = Object::New(Isolate::GetCurrent());
    obj->Set(String::NewFromUtf8(Isolate::GetCurrent(), "x"), v8::Number::New(Isolate::GetCurrent(), canvasRect.getUpperLeft().x));
    obj->Set(String::NewFromUtf8(Isolate::GetCurrent(), "y"), v8::Number::New(Isolate::GetCurrent(), canvasRect.getUpperLeft().y));
    args.GetReturnValue().Set(obj);
}

void JSComponent::v8GetCanvasInputs(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() != 1) {
        console() << "please call ceGetInputs as follows: ceGetInputs(max_distance)"<<endl;
        args.GetReturnValue().Set(0);
        return;
    }
    
    float maxDistance = args[0]->NumberValue();
    vector<InputNode*> inodes = canvas->getInputNodesAtArea(canvasRect.getCenter(), maxDistance);
    
    HandleScope scope(args.GetIsolate());

    Handle<Array> array = Array::New(Isolate::GetCurrent());
    for (int i=0; i<inodes.size(); i++)
    {
        Handle<Object> inputObject = Object::New(Isolate::GetCurrent());
        inputObject->Set(String::NewFromUtf8(Isolate::GetCurrent(), "id"), v8::Number::New(Isolate::GetCurrent(), inodes[i]->getId()));
        inputObject->Set(String::NewFromUtf8(Isolate::GetCurrent(), "name"), v8::String::NewFromUtf8(Isolate::GetCurrent(), inodes[i]->getName().data()));
        inputObject->Set(String::NewFromUtf8(Isolate::GetCurrent(), "distance"), v8::Number::New(Isolate::GetCurrent(), (canvasRect.getCenter() - inodes[i]->getCanvasPos()).length()));
        inputObject->Set(String::NewFromUtf8(Isolate::GetCurrent(), "connected"), v8::Boolean::New(Isolate::GetCurrent(), inodes[i]->isConnected()));
        array->Set(i, inputObject);
    }
    
    args.GetReturnValue().Set(array);
}

void JSComponent::v8ConnectOutput(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() != 2) {
        console() << "please call ceConnectToInput as follows: ceConnectToInput(output_index, input_id)"<<endl;
        args.GetReturnValue().Set(false);
        return;
    }
    
    HandleScope scope(args.GetIsolate());
    
    int outputIndex = args[0]->NumberValue();
    int inputId = args[1]->NumberValue();
    
    if (outputIndex >= outputNodes.size()) {
        console() << "warning: no such output node index: "<<outputIndex<<endl;
        args.GetReturnValue().Set(false);
        return;
    }
    
    if (outputNodes[outputIndex]->isConnected()) {
        args.GetReturnValue().Set(false);
        return;
    }
    
    canvas->makeConnection(outputNodes[outputIndex], inputId);
    args.GetReturnValue().Set(true);
}

void JSComponent::v8DisconnectOutput(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() != 1) {
        console() << "please call ceDisconnectOutput as follows: ceConnectToInput(output_index)"<<endl;
        args.GetReturnValue().Set(false);
        return;
    }
    
    HandleScope scope(args.GetIsolate());
    
    int outputIndex = args[0]->NumberValue();
    
    if (outputIndex >= outputNodes.size()) {
        console() << "warning: no such output node index: "<<outputIndex<<endl;
        args.GetReturnValue().Set(false);
        return;
    }
    
    if (!outputNodes[outputIndex]->isConnected()) {
        args.GetReturnValue().Set(false);
        return;
    }
    
    canvas->disconnectNode(outputNodes[outputIndex]);
    args.GetReturnValue().Set(true);
}

void JSComponent::v8Log(const FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() != 1) {
        console() << "please call ceLog as follows: ceLog(string)"<<endl;
        return;
    }
    
    HandleScope scope(args.GetIsolate());
    v8::String::Utf8Value str(args[0]->ToString());
    console() << compName << ": " << (*str) << endl;
}


/*****************************************************************************/
/*****************************************************************************/
/* V8 CALLBACKS                                                              */
/*****************************************************************************/
/*****************************************************************************/

#define V8_STATIC_CALLBACK(X) \
    void JSComponent::(X)CB(const FunctionCallbackInfo<v8::Value> &args) \
    { \
        Local<External> wrap = Local<External>::Cast(args.Data()); \
        JSComponent *comp = (JSComponent*)wrap->Value(); \
        return comp->X(args); \
    }

void JSComponent::v8InitCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8Init(args);
}

void JSComponent::v8SetGuiSizeCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8SetGuiSize(args);
}

void JSComponent::v8DrawEllipseCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8DrawEllipse(args);
    
}

void JSComponent::v8DrawLineCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8DrawLine(args);
    
}

void JSComponent::v8DrawRectCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8DrawRect(args);
}

void JSComponent::v8DrawStringCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8DrawString(args);
}

void JSComponent::v8SetBrightnessCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8SetBrightness(args);
}

void JSComponent::v8SetBWCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8SetBW(args);
}

void JSComponent::v8SetHueCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8SetHue(args);
}

void JSComponent::v8NoiseCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8Noise(args);
}

void JSComponent::v8SetOffsetCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8SetOffset(args);
}

void JSComponent::v8GetPositionCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8GetPosition(args);
}

void JSComponent::v8GetCanvasInputsCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8GetCanvasInputs(args);
}

void JSComponent::v8ConnectOutputCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8ConnectOutput(args);
}

void JSComponent::v8DisconnectOutputCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8DisconnectOutput(args);
}

void JSComponent::v8LogCB(const FunctionCallbackInfo<v8::Value> &args)
{
    Local<External> wrap = Local<External>::Cast(args.Data());
    JSComponent *comp = (JSComponent*)wrap->Value();
    return comp->v8Log(args);
}

