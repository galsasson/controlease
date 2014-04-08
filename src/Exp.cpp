//
//  Exp.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "Exp.h"

Exp::Exp(Canvas *c) : CanvasComponent(c)
{
    type = ComponentType::COMPONENT_TYPE_EXP;
    isEditing = false;
    showOutputPlus = true;
    showInputPlus = true;
}

Exp::~Exp()
{
    delete codeInput;
    
    pFunction.Reset();
    pContext.Reset();
}

void Exp::initNew(Vec2f pos)
{
    CanvasComponent::initNew(pos);
    setSize(Vec2f(150, 50));
    setName("Exp");
    
    ivals.push_back(0);
    addNewInputNode();
    addNewOutputNode();
    
//    textEditRect = Rectf(16, 22, localRect.getWidth()-20, localRect.getHeight()-2);
    codeInput = new TextInput(Vec2f(16, 23), Vec2f(localRect.getWidth()-32, 14), true);
    codeInput->onReturn(boost::bind(&Exp::inputEnterPressed, this));
    
    resizeComponent();
}

void Exp::initFromXml(const XmlTree& xml)
{
    
}

void Exp::inputEnterPressed()
{
    isEditing = false;

    // remove old code function
    if (!pFunction.IsEmpty()) {
        pFunction.Reset();
    }
    
    std::ostringstream str;
    str << "var time=0; var update = function() {"<<codeInput->getValue()<<"}";

    compileAndRun(str.str());
}

void Exp::update()
{
    if (isEditing)
    {
        codeInput->update();
        resizeComponent();
    }

    // call 'update' function
    runCompiledScript();
}

void Exp::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(localRect, 2);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(localRect, 2);
    
    // draw title
    ResourceManager::getInstance().getTextureFont()->drawString("Expression", titleRect);
    gl::drawLine(Vec2f(0, titleRect.y2), Vec2f(localRect.getWidth(), titleRect.y2));
    
    // draw + sign
    gl::draw(ResourceManager::getInstance().getPlusTexture(), inputPlusRect.getUpperLeft());
    gl::draw(ResourceManager::getInstance().getPlusTexture(), outputPlusRect.getUpperLeft());

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
    
    gl::color(0.5, 0.5, 0.5);
    gl::drawStrokedRect(Rectf(Vec2f(0, 0), canvasRect.getSize() + Vec2f(8, 8)));
    
    glPopAttrib();
        
    gl::popMatrices();
}

void Exp::mouseDown(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    isEditing = false;
    
    if (outputPlusRect.contains(local)) {
        // add another output
        addNewOutputNode();
        resizeComponent();
        return;
    }
    else if (inputPlusRect.contains(local)){
        // add another input
        addNewInputNode();
        ivals.push_back(0);
        resizeComponent();
        return;
    }
    else if (codeInput->contains(local)) {
        isEditing = true;
        return;
    }
    
    compDragAnchor = event.getPos();
}

void Exp::mouseDrag(const cease::MouseEvent& event)
{
    canvasRect += event.getPos() - compDragAnchor;
    compDragAnchor = event.getPos();
    applyBorders();
}

bool Exp::isDragPoint(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    
    return titleRect.contains(local);
}

bool Exp::isHotspot(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    
    return titleRect.contains(local) ||
        inputPlusRect.contains(local) ||
        outputPlusRect.contains(local) ||
        codeInput->contains(local);
}

KeyboardListener* Exp::getCurrentKeyboardListener()
{
    if (isEditing) {
        return codeInput;
    }
    
    return NULL;
}

void Exp::resizeComponent()
{
    pack(codeInput->getTextSize().x + 32, codeInput->getTextSize().y + 30);
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


void Exp::v8InGetter(uint32_t index, const PropertyCallbackInfo<v8::Value> &info)
{
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Exp *component = (Exp*)wrap->Value();
    ReturnValue<Value> ret = info.GetReturnValue();
    if (component->ivals.size() > index) {
        ret.Set(component->ivals[index]);
    }
    else {
        // return 0 if no input exists
        ret.Set(0);
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
    
    // find 'update' function
    Handle<String> function_name = String::NewFromUtf8(Isolate::GetCurrent(), "update");
    Handle<Value> function_val = context->Global()->Get(function_name);
    if (!function_val->IsFunction()) {
        console() << "could not find 'update' function."<<endl;
        return;
    }
    
    Handle<Function> updateFunction = Handle<Function>::Cast(function_val);
    pFunction.Reset(Isolate::GetCurrent(), updateFunction);
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
    
    args.GetReturnValue().Set(lmap(args[0]->NumberValue(),
                                   args[1]->NumberValue(),
                                   args[2]->NumberValue(),
                                   args[3]->NumberValue(),
                                   args[4]->NumberValue()));
}
