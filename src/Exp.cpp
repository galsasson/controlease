//
//  Exp.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "Exp.h"

Exp::Exp(Vec2f p, Vec2f s)
{
    canvasRect = Rectf(p, p+s);
    initInterface(s);
    
    isEditing = false;
}

Exp::~Exp()
{
    for (int i=0; i<inputNodes.size(); i++) {
        delete inputNodes[i];
    }
    for (int i=0; i<outputNodes.size(); i++) {
        delete outputNodes[i];
    }
    
    pFunction.Reset();
    pContext.Reset();
}

void Exp::initInterface(Vec2f size)
{
    rect = Rectf(Vec2f(0, 0), size);
    titleRect = Rectf(15, 2, size.x-20, 20);
    outPlusRect = Rectf(size.x-10, 6, size.x-2, 14);
    inPlusRect = Rectf(2, 6, 10, 14);
    inputNodes.push_back(new InputNode(0, this, Vec2f(6, titleRect.y2 + 6)));
    outputNodes.push_back(new OutputNode(0, this, Vec2f(size.x - 6, titleRect.y2 + 6)));
    nextInputPos = Vec2f(6, titleRect.y2 + 15);
    nextOutputPos = Vec2f(size.x - 6, titleRect.y2 + 15);
    ivals.push_back(0);
    
    textEditRect = Rectf(22, 22, size.x-20, size.y-2);
    codeInput = new TextInput(Vec2f(25, 25), Vec2f(size.x-40, 14), true);
    codeInput->onReturn(boost::bind(&Exp::inputEnterPressed, this));
}

void Exp::inputEnterPressed()
{
    isEditing = false;

    // remove old code function
    if (!pFunction.IsEmpty()) {
        pFunction.Reset();
    }
    
    std::ostringstream str;
    str << "var time=0; var loop = function() {"<<codeInput->getValue()<<"}";

    compileAndRun(str.str());
}

void Exp::update()
{
    codeInput->update();
    resizeComponent();

    runCompiledScript();
}

void Exp::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(rect, 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(rect, 2);
    
    // draw title
    ResourceManager::getInstance().getTextureFont()->drawString("Expression", titleRect);
    gl::drawLine(Vec2f(0, titleRect.y2), Vec2f(rect.getWidth(), titleRect.y2));
    
    // draw + sign
    gl::drawLine(Vec2f(inPlusRect.x1, inPlusRect.y1 + inPlusRect.getHeight()/2), Vec2f(inPlusRect.x2, inPlusRect.y1 + inPlusRect.getHeight()/2));
    gl::drawLine(Vec2f(inPlusRect.x1 + inPlusRect.getWidth()/2, inPlusRect.y1), Vec2f(inPlusRect.x1 + inPlusRect.getWidth()/2, inPlusRect.y2));
    // draw + sign
    gl::drawLine(Vec2f(outPlusRect.x1, outPlusRect.y1 + outPlusRect.getHeight()/2), Vec2f(outPlusRect.x2, outPlusRect.y1 + outPlusRect.getHeight()/2));
    gl::drawLine(Vec2f(outPlusRect.x1 + outPlusRect.getWidth()/2, outPlusRect.y1), Vec2f(outPlusRect.x1 + outPlusRect.getWidth()/2, outPlusRect.y2));

    // draw text input
    codeInput->draw();
    
    gl::color(0, 0, 0);
    // draw nodes
    for (int i=0; i<inputNodes.size(); i++) {
        inputNodes[i]->draw();
    }
    for (int i=0; i<outputNodes.size(); i++) {
        outputNodes[i]->draw();
    }
    
    gl::popMatrices();
}

void Exp::drawOutline()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());

    if (isEditing) {
        codeInput->drawInFocus();
    }

    gl::translate(Vec2f(-4, -4));

    glPushAttrib(GL_ENABLE_BIT);
    glLineStipple(1, 0xff00);
    gl::enable(GL_LINE_STIPPLE);
    
    gl::color(0, 0, 0);
    gl::drawStrokedRect(Rectf(Vec2f(0, 0), canvasRect.getSize() + Vec2f(8, 8)));
    
    glPopAttrib();
        
    gl::popMatrices();
}

void Exp::translate(Vec2f offset)
{
    canvasRect += offset;
}

Rectf Exp::getBounds()
{
    return canvasRect;
}

void Exp::mouseDown(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    isEditing = false;
    
    if (outPlusRect.contains(local)) {
        // add another output
        outputNodes.push_back(new OutputNode(outputNodes.size(), this, nextOutputPos));
        nextOutputPos.y += 9;
        resizeComponent();
        return;
    }
    else if (inPlusRect.contains(local)){
        // add another input
        inputNodes.push_back(new InputNode(inputNodes.size(), this, nextInputPos));
        ivals.push_back(0);
        nextInputPos.y += 9;
        resizeComponent();
        return;
    }
    else if (textEditRect.contains(local)) {
        isEditing = true;
        return;
    }
    
    compDragAnchor = event.getPos();
}

void Exp::mouseDrag(cease::MouseEvent event)
{
    canvasRect += event.getPos() - compDragAnchor;
    compDragAnchor = event.getPos();
    applyBorders();
}

void Exp::mouseUp( cease::MouseEvent event)
{
}

void Exp::mouseWheel( cease::MouseEvent event ) {}
void Exp::mouseMove( cease::MouseEvent event ) {}

bool Exp::isDragPoint(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    return titleRect.contains(local);
}

bool Exp::isHotspot(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    
    return titleRect.contains(local) ||
        inPlusRect.contains(local) ||
        outPlusRect.contains(local) ||
        textEditRect.contains(local);
}

ConnectionResult* Exp::getConnectionStart(cease::MouseEvent event)
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

ConnectionResult* Exp::getConnectionEnd(cease::MouseEvent event)
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

vector<Node*> Exp::getInputNodes()
{
    vector<Node*> inputs;
    
    for (int i=0; i<inputNodes.size(); i++) {
        inputs.push_back((Node*)inputNodes[i]);
    }
    
    return inputs;
}

vector<Node*> Exp::getOutputNodes()
{
    vector<Node*> outputs;
    
    for (int i=0; i<outputNodes.size(); i++) {
        outputs.push_back((Node*)outputNodes[i]);
    }
    
    return outputs;
}

KeyboardListener* Exp::getCurrentKeyboardListener()
{
    if (isEditing) {
        return codeInput;
    }
    
    return NULL;
}

Vec2f Exp::getCanvasPos()
{
    return canvasRect.getUpperLeft();
}

bool Exp::contains(Vec2f p)
{
    return canvasRect.contains(p);
}

float Exp::getValue(int i)
{
    // this returns the input values
    if (i >= ivals.size()) {
        return 0;
    }
    
    return ivals[i];
}

void Exp::setValue(int i, float v)
{
    if (i >= ivals.size()) {
        return;
    }
    
    ivals[i] = v;
}

Vec2f Exp::getLocalCoords(Vec2f p)
{
    return p-canvasRect.getUpperLeft();
}

Vec2f Exp::getCanvasCoords(Vec2f p)
{
    return canvasRect.getUpperLeft() + p;
}

void Exp::applyBorders()
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

void Exp::resizeComponent()
{
    int maxNodes = max(inputNodes.size(), outputNodes.size());
    float heightNeededForNodes = 26 + 9*maxNodes;
    float heightNeededForText = 26 + codeInput->getTextSize().y;
    
    float neededHeight = max(heightNeededForNodes, heightNeededForText);
    
    canvasRect.y2 = canvasRect.y1 + neededHeight;
    rect.y2 = neededHeight;
}




void Exp::v8InGetter(uint32_t index, const PropertyCallbackInfo<v8::Value> &info)
{
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Exp *component = (Exp*)wrap->Value();
    ReturnValue<Value> ret = info.GetReturnValue();
    if (component->ivals.size() > index) {
        ret.Set(component->ivals[index]);
    }
}

void Exp::v8OutGetter(uint32_t index, const PropertyCallbackInfo<v8::Value> &info)
{
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Exp *component = (Exp*)wrap->Value();
    ReturnValue<Value> ret = info.GetReturnValue();
    if (component->ivals.size() > index) {
        ret.Set(component->outputNodes[index]->getLastVal());
    }
}

void Exp::v8OutSetter(uint32_t index, Local<Value> val, const PropertyCallbackInfo<Value>& info)
{
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Exp *component = (Exp*)wrap->Value();
    if (component->outputNodes.size() > index) {
        component->outputNodes[index]->updateVal(val->NumberValue());
    }
    ReturnValue<Value> ret = info.GetReturnValue();
    ret.Set(true);
}

void Exp::compileAndRun(std::string code)
{
    /* compile the script */
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(Isolate::GetCurrent());

    // add the map function
    Handle<ObjectTemplate> global = ObjectTemplate::New(Isolate::GetCurrent());
    global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "map"), FunctionTemplate::New(Isolate::GetCurrent(), v8Map));
    
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
    
    // find 'loop' function
    Handle<String> process_name = String::NewFromUtf8(Isolate::GetCurrent(), "loop");
    Handle<Value> process_val = context->Global()->Get(process_name);
    if (!process_val->IsFunction()) {
        console() << "could not find 'loop' function."<<endl;
        return;
    }
    
    Handle<Function> loopFunction = Handle<Function>::Cast(process_val);
    pFunction.Reset(Isolate::GetCurrent(), loopFunction);
}

void Exp::runCompiledScript()
{
    if (pFunction.IsEmpty()) {
        return;
    }
    
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(Isolate::GetCurrent());
    
    Local<Context> context = Local<Context>::New(Isolate::GetCurrent(), pContext);
    
    Context::Scope context_scope(context);
    
    Local<Function> loopFunction = Local<Function>::New(Isolate::GetCurrent(), pFunction);
    loopFunction->Call(context->Global(), 0, NULL);
}

void Exp::v8Map(const FunctionCallbackInfo<v8::Value> &args)
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
