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
    setType(ComponentType::COMPONENT_TYPE_EXP);
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
    
    addNewInputNode();
    addNewOutputNode();
    
    codeInput = new TextInput();
    codeInput->initNew(Vec2f(16, 23), Vec2f(localRect.getWidth()-32, 14), true);
    codeInput->onReturn(boost::bind(&Exp::inputEnterPressed, this));
    
    resizeComponent();
}

void Exp::initFromXml(const XmlTree& xml, bool createNodes)
{
    CanvasComponent::initFromXml(xml);
    
    codeInput = new TextInput();
    codeInput->initFromXml(xml.getChild("TextInput"));
    codeInput->onReturn(boost::bind(&Exp::inputEnterPressed, this));

    resizeComponent();
    
    prepareJSScript();
}

XmlTree Exp::getXml()
{
    XmlTree xml = CanvasComponent::getXml();
    
    xml.push_back(codeInput->getXml());
    
    return xml;
}

void Exp::inputEnterPressed()
{
    isEditing = false;

    prepareJSScript();
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
    ResourceManager::getInstance().getTextureFont()->drawString(name, titleRect);
    gl::drawLine(Vec2f(2, titleRect.y2), Vec2f(localRect.getWidth()-4, titleRect.y2));
    
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
//        ivals.push_back(0);
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
    if (i >= inputNodes.size()) {
        return 0;
    }
    
    return inputNodes[i]->getLastVal();
//    if (i >= ivals.size()) {
//        return 0;
//    }
//    
//    return ivals[i];
}

void Exp::setValue(int i, float v)
{
    runCompiledScript();
    
//    if (i >= ivals.size()) {
//        return;
//    }
//    
//    ivals[i] = v;
}

void Exp::prepareJSScript()
{
    // remove old code function
    if (!pFunction.IsEmpty()) {
        pFunction.Reset();
    }
    
    std::ostringstream str;
    str << "var time=0; var val=[]; var update = function() {"<<codeInput->getValue()<<"}";
    
    compileAndRun(str.str());
}

void Exp::v8InGetter(uint32_t index, const PropertyCallbackInfo<v8::Value> &info)
{
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Exp *component = (Exp*)wrap->Value();
    ReturnValue<Value> ret = info.GetReturnValue();
    if (component->inputNodes.size() > index) {
        ret.Set(component->inputNodes[index]->getLastVal());
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
    if (component->inputNodes.size() > index) {
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
    v8::Locker l(ResourceManager::mainIsolate);
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(ResourceManager::mainIsolate);

    // add the map function
    Handle<ObjectTemplate> global = ObjectTemplate::New(ResourceManager::mainIsolate);
    global->Set(String::NewFromUtf8(ResourceManager::mainIsolate, "map"), FunctionTemplate::New(ResourceManager::mainIsolate, v8Map));
    
    // add interceptors to inputs and outputs
    Handle<ObjectTemplate> globalIn = ObjectTemplate::New(ResourceManager::mainIsolate);
    globalIn->SetInternalFieldCount(1);
    globalIn->SetIndexedPropertyHandler(v8InGetter);
    Handle<ObjectTemplate> globalOut = ObjectTemplate::New(ResourceManager::mainIsolate);
    globalOut->SetInternalFieldCount(1);
    globalOut->SetIndexedPropertyHandler(v8OutGetter, v8OutSetter);
    
    // Create a new context.
    Handle<Context> context = Context::New(ResourceManager::mainIsolate, NULL, global);
    
    // Enter the context for compiling and running the hello world script.
    Context::Scope context_scope(context);
    
    Local<Object> objIn = globalIn->NewInstance();
    objIn->SetInternalField(0, External::New(ResourceManager::mainIsolate, this));
    Local<Object> objOut = globalOut->NewInstance();
    objOut->SetInternalField(0, External::New(ResourceManager::mainIsolate, this));
    context->Global()->Set(String::NewFromUtf8(ResourceManager::mainIsolate, "inn"), objIn);
    context->Global()->Set(String::NewFromUtf8(ResourceManager::mainIsolate, "out"), objOut);
    
    pContext.Reset(ResourceManager::mainIsolate, context);
    
    // Create a string containing the JavaScript source code.
    Handle<String> source = String::NewFromUtf8(ResourceManager::mainIsolate, code.c_str());
    
    // Compile the source code.
    Handle<Script> localScript = Script::Compile(source);
    if (localScript.IsEmpty()) {
        return;
    }
    
    localScript->Run();
    
    // find 'update' function
    Handle<String> function_name = String::NewFromUtf8(ResourceManager::mainIsolate, "update");
    Handle<Value> function_val = context->Global()->Get(function_name);
    if (!function_val->IsFunction()) {
        console() << "could not find 'update' function."<<endl;
        return;
    }
    
    Handle<Function> updateFunction = Handle<Function>::Cast(function_val);
    pFunction.Reset(ResourceManager::mainIsolate, updateFunction);
    
    v8::Unlocker ul(ResourceManager::mainIsolate);
}

void Exp::runCompiledScript()
{
    if (pFunction.IsEmpty()) {
        return;
    }
 
    v8::Locker l(ResourceManager::mainIsolate);
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(ResourceManager::mainIsolate);
    
    Local<Context> context = Local<Context>::New(ResourceManager::mainIsolate, pContext);
    
    Context::Scope context_scope(context);
    
    Local<Function> loopFunction = Local<Function>::New(ResourceManager::mainIsolate, pFunction);
    loopFunction->Call(context->Global(), 0, NULL);
    
    v8::Unlocker ul(ResourceManager::mainIsolate);
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
