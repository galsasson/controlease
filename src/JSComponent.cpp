//
//  JSComponent.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "JSComponent.h"

JSComponent::JSComponent(Vec2f p, string filename)
{
    compName = "javascript";
    Vec2f size(10, 0);
    canvasRect = Rectf(p, size);
    localRect = Rectf(Vec2f(0, 0), Vec2f(10, 30));
    titleRect = Rectf(4, 2, localRect.x2, 20);
    nextInputPos = Vec2f(6, 26);
    nextOutputPos = Vec2f(localRect.x2 - 6, 26);
    jsRect = Rectf(15, 25, 0, 0);
    jsFilename = filename;
    
    initComponent();
}

JSComponent::~JSComponent()
{
    for (int i=0; i<inputNodes.size(); i++) {
        delete inputNodes[i];
    }
    for (int i=0; i<outputNodes.size(); i++) {
        delete outputNodes[i];
    }
    
    pSetupFunc.Reset();
    pUpdateFunc.Reset();
    pDrawFunc.Reset();
    pMouseDownFunc.Reset();
    pMouseUpFunc.Reset();
    pContext.Reset();
}

void JSComponent::initNodes(int nIns, int nOuts)
{
    for (int i=0; i<nIns; i++)
    {
        inputNodes.push_back(new InputNode(i, this, nextInputPos));
        nextInputPos.y += 9;
        ivals.push_back(0);
    }
    for (int i=0; i<nOuts; i++)
    {
        outputNodes.push_back(new OutputNode(i, this, nextOutputPos));
        nextOutputPos.y += 9;
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
    callV8Function(pDrawFunc);
    
    gl::popMatrices();
}

void JSComponent::drawOutline()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());

    gl::translate(Vec2f(-4, -4));

    glPushAttrib(GL_ENABLE_BIT);
    glLineStipple(1, 0xff00);
    gl::enable(GL_LINE_STIPPLE);
    
    gl::color(0, 0, 0);
    gl::drawStrokedRect(Rectf(Vec2f(0, 0), canvasRect.getSize() + Vec2f(8, 8)));
    
    glPopAttrib();
        
    gl::popMatrices();
}

void JSComponent::translate(Vec2f offset)
{
    canvasRect += offset;
}

Rectf JSComponent::getBounds()
{
    return canvasRect;
}

void JSComponent::mouseDown(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());

    if (titleRect.contains(local)) {
        // drag the component
        compDragAnchor = event.getPos();
    }
    else {
        // call mouse down of js component
    }
}

void JSComponent::mouseDrag(cease::MouseEvent event)
{
    canvasRect += event.getPos() - compDragAnchor;
    compDragAnchor = event.getPos();
    applyBorders();
}

void JSComponent::mouseUp( cease::MouseEvent event)
{
    // call mouse up of js component
}

void JSComponent::mouseWheel( cease::MouseEvent event ) {}
void JSComponent::mouseMove( cease::MouseEvent event ) {}

bool JSComponent::isDragPoint(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    return titleRect.contains(local);
}

bool JSComponent::isHotspot(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    return titleRect.contains(local) ||
            jsRect.contains(local);
}

ConnectionResult* JSComponent::getConnectionStart(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());

    for (int i=0; i<inputNodes.size(); i++)
    {
        if (inputNodes[i]->contains(local)) {
            if (inputNodes[i]->isConnected()) {
                return new ConnectionResult(TYPE_DISCONNECT_INPUT, inputNodes[i]);
            }
        }
    }
    for (int i=0; i<outputNodes.size(); i++)
    {
        if (outputNodes[i]->contains(local)) {
            if (outputNodes[i]->isConnected()) {
                return new ConnectionResult(TYPE_DISCONNECT_OUTPUT, outputNodes[i]);
            }
            else {
                return new ConnectionResult(TYPE_OUTPUT, outputNodes[i]);
            }
        }
    }

    return NULL;
}

ConnectionResult* JSComponent::getConnectionEnd(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());

    for (int i=0; i<inputNodes.size(); i++)
    {
        if (inputNodes[i]->contains(local)) {
            if (!inputNodes[i]->isConnected()) {
                return new ConnectionResult(TYPE_INPUT, inputNodes[i]);
            }
        }
    }
    for (int i=0; i<outputNodes.size(); i++)
    {
        if (outputNodes[i]->contains(local)) {
            if (!outputNodes[i]->isConnected()) {
                return new ConnectionResult(TYPE_OUTPUT, outputNodes[i]);
            }
        }
    }
    
    return NULL;
}

vector<Node*> JSComponent::getInputNodes()
{
    vector<Node*> inputs;
    
    for (int i=0; i<inputNodes.size(); i++) {
        inputs.push_back((Node*)inputNodes[i]);
    }
    
    return inputs;
}

vector<Node*> JSComponent::getOutputNodes()
{
    vector<Node*> outputs;
    
    for (int i=0; i<outputNodes.size(); i++) {
        outputs.push_back((Node*)outputNodes[i]);
    }
    
    return outputs;
}

KeyboardListener* JSComponent::getCurrentKeyboardListener()
{
    return NULL;
}

Vec2f JSComponent::getCanvasPos()
{
    return canvasRect.getUpperLeft();
}

bool JSComponent::contains(Vec2f p)
{
    return canvasRect.contains(p);
}

float JSComponent::getValue(int i)
{
    // this returns the input values
    if (i >= ivals.size()) {
        return 0;
    }
    
    return ivals[i];
}

void JSComponent::setValue(int i, float v)
{
    if (i >= ivals.size()) {
        return;
    }
    
    ivals[i] = v;
}

Vec2f JSComponent::getLocalCoords(Vec2f p)
{
    return p-canvasRect.getUpperLeft();
}

Vec2f JSComponent::getCanvasCoords(Vec2f p)
{
    return canvasRect.getUpperLeft() + p;
}

void JSComponent::applyBorders()
{
    float x1 = canvasRect.getUpperLeft().x;
    float x2 = canvasRect.getUpperRight().x;
    float y1 = canvasRect.getUpperLeft().y;
    float y2 = canvasRect.getLowerRight().y;
    
    if (x1 < 0) {
        canvasRect += Vec2f(-x1, 0);
    }
    else if (x2 > CANVAS_WIDTH) {
        canvasRect -= Vec2f(x2-CANVAS_WIDTH, 0);
    }
    
    if (y1 < 0) {
        canvasRect += Vec2f(0, -y1);
    }
    else if (y2 > CANVAS_HEIGHT) {
        canvasRect -= Vec2f(0, y2-CANVAS_HEIGHT);
    }
    
}

void JSComponent::resizeComponent()
{
    int maxNodes = max(inputNodes.size(), outputNodes.size());
    float heightNeededForNodes = 26 + 9*maxNodes;
    float heightNeededForComponent = jsRect.getHeight();
    float neededHeight = max(heightNeededForNodes, heightNeededForComponent);
    
    float neededWidth = jsRect.getWidth() + 30;
    
    canvasRect.y2 = canvasRect.y1 + neededHeight;
    localRect.y2 = neededHeight;
    canvasRect.x2 = canvasRect.x1 + neededWidth;
    localRect.x2 = neededWidth;
    titleRect.x2 = neededWidth;
    
    // push outputs to the right
    for (int i=0; i<outputNodes.size(); i++)
    {
        outputNodes[i]->pos.x = localRect.x2 - 6;
    }
    
}

void JSComponent::initComponent()
{
    stringstream ss;
    ss << getAppPath().c_str()<<"/Contents/Resources/components/" << jsFilename;
    ifstream jsfile(ss.str());
    if (!jsfile.is_open()) {
        console() << "error: cannot find file '"<<ss.str()<<"'"<<endl;
        return;
    }
    
    stringstream buffer;
    buffer << jsfile.rdbuf();
    compileAndRun(buffer.str());
}

void JSComponent::v8InGetter(uint32_t index, const PropertyCallbackInfo<v8::Value> &info)
{
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    JSComponent *comp = (JSComponent*)wrap->Value();
    ReturnValue<Value> ret = info.GetReturnValue();
    if (comp->ivals.size() > index) {
        ret.Set(comp->ivals[index]);
    }
    else {
        // return 0 if no input exists
        ret.Set(0);
    }
}

void JSComponent::v8OutGetter(uint32_t index, const PropertyCallbackInfo<v8::Value> &info)
{
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    JSComponent *comp = (JSComponent*)wrap->Value();
    ReturnValue<Value> ret = info.GetReturnValue();
    if (comp->ivals.size() > index) {
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
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "map"), FunctionTemplate::New(Isolate::GetCurrent(), v8Map));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "init"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8InitCB, External::New(Isolate::GetCurrent(), this)));
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "setGuiSize"), FunctionTemplate::New(Isolate::GetCurrent(), &JSComponent::v8SetGuiSizeCB, External::New(Isolate::GetCurrent(), this)));
    
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
    if (!getFunction(context, "draw", pDrawFunc)) {
        return;
    }
    
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

void JSComponent::scriptRunUpdate()
{
    if (pUpdateFunc.IsEmpty()) {
        return;
    }
    
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(Isolate::GetCurrent());
    
    Local<Context> context = Local<Context>::New(Isolate::GetCurrent(), pContext);
    
    Context::Scope context_scope(context);
    
    Local<Function> updateFunction = Local<Function>::New(Isolate::GetCurrent(), pUpdateFunc);
    updateFunction->Call(context->Global(), 0, NULL);
}

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
    
    initGUI(Vec2f(args[0]->NumberValue(), args[1]->NumberValue()));
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
