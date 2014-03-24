//
//  OscController.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#include "OscController.h"

void printMessage(osc::Message message);

OscController::OscController(Canvas *c, Vec2f _pos) : CanvasComponent(c)
{
    titleRect = Rectf(5, 0, 100, 20);
    localRect = Rectf(0, 0, 100, 40);
    canvasRect = Rectf(_pos, _pos+localRect.getSize());
    nextInputPos = Vec2f(6, 28);
    nextOutputPos = Vec2f(localRect.x2 - 6, 28);
    
    textInputRect = Rectf(7, 22, 100, 36);
    addressInput = new TextInput(Vec2f(7, 22), Vec2f(200, 14));
    addressInput->onReturn(boost::bind(&OscController::addressInputSet, this));

    listenPort = 8000;
    name = "OscController";

    isEditing = true;
    connected = false;
}

OscController::~OscController()
{
    // delete all inputs
    for (int i=0; i<inputs.size(); i++)
    {
        delete inputs[i];
    }
    for (int i=0; i<outputs.size(); i++)
    {
        delete outputs[i];
    }
    
    if (connected) {
        connected = false;
        handleMsg.join();
    }
    
    delete addressInput;
}

void OscController::setupConnection(int port)
{
    listenPort = port;
    
    connect();
}

void OscController::addressInputSet(void)
{
    string textVal = addressInput->getValue();
    
    int listenPort;
    int outPort = 0;
    
    int collonIndex = textVal.find(':');
    if (collonIndex < 0) {
        listenPort = atoi(textVal.c_str());
    }
    else {
        listenPort = atoi(textVal.substr(0, collonIndex).c_str());
        outPort = atoi(textVal.substr(collonIndex+1, textVal.size()).c_str());
    }
    
    console() << "listening to: " << listenPort << ":" << outPort<<endl;
    
    setupConnection(listenPort);
}

void OscController::update()
{
    if (!connected && isEditing) {
        addressInput->update();
        return;
    }
}

void OscController::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());

    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(localRect, 3);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(localRect, 3);

    ResourceManager::getInstance().getTextureFont()->drawString(name, titleRect);
    gl::drawLine(Vec2f(0, 20), Vec2f(localRect.x2, 20));

    if (connected)
    {
        // draw all input nodes
        for (int i=0; i<inputNodes.size(); i++)
        {
            inputNodes[i]->draw();
        }
        // draw the output nodes
        for (int i=0; i<outputNodes.size(); i++)
        {
            outputNodes[i]->draw();
            ResourceManager::getInstance().getTextureFont()->drawString(outputNodes[i]->name, outputNodes[i]->pos - Vec2f(outputNodes[i]->nameSize.x+5, -3));
        }
    }
    else {
        addressInput->draw();
    }
    
    gl::popMatrices();
}

void OscController::drawOutline()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());
    if (!connected)
    {
        addressInput->drawInFocus();
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

void OscController::mouseDown(cease::MouseEvent event)
{
    Vec2f local = toLocal(event.getPos());
    isEditing = false;
    
    if (!connected) {
        if (textInputRect.contains(local)) {
            isEditing = true;
            return;
        }
    }

    prevMouse = event.getPos();
}

void OscController::mouseUp(cease::MouseEvent event)
{
}

void OscController::mouseWheel(cease::MouseEvent event)
{
}

void OscController::mouseMove(cease::MouseEvent event)
{
    
}

void OscController::mouseDrag(cease::MouseEvent event)
{
    canvasRect += event.getPos() - prevMouse;
    prevMouse = event.getPos();
    
    applyBorders();
}

bool OscController::isHotspot(cease::MouseEvent event)
{
    Vec2f local = toLocal(event.getPos());
    return titleRect.contains(local) || (!connected && textInputRect.contains(local));
}

bool OscController::isDragPoint(cease::MouseEvent event)
{
    return titleRect.contains(toLocal(event.getPos()));
}

KeyboardListener* OscController::getCurrentKeyboardListener()
{
    if (!connected && isEditing) {
        return addressInput;
    }
    
    return NULL;
}

float OscController::getValue(int i)
{
    return outputNodes[i]->getLastVal();
//    return inputs[i]->getValue();
}

void OscController::setValue(int i, float v)
{
    console() << "warning OscController::setValue is not implemented"<<endl;
//    inputs[i]->sendVal(v);
}

void OscController::connect()
{
    oscListener.setup(listenPort);
    // handle incoming messages from the OscController
    connected = true;
//    int id = oscListener.registerMessageReceived(this, &OscController::handleMessage);
    handleMsg = std::thread(&OscController::handleMessages, this);

}

void OscController::handleMessages()
{
    while (connected)
    {
        osc::Message message;
        try
        {
        while( oscListener.hasWaitingMessages() )
        {
            oscListener.getNextMessage( &message );
            
            if (message.getNumArgs() < 1) {
                printMessage(message);
                continue;
            }
            
            char *s= NULL;
            s = strstr(message.getAddress().c_str(), "xy");
            if (s) {
                handleTwoValues(message);
                continue;
            }
            else {
                handleOneValue(message);
                continue;
            }
        }
        }
        catch (cinder::Exception e) {
            console() << "error parsing message: " << e.what() << endl;
            printMessage(message);
        }
        usleep(5000);
    }
}

void OscController::handleOneValue(osc::Message &msg)
{
    std::string name = msg.getAddress();
    for (int i=0; i<outputNodes.size(); i++)
    {
        if (outputNodes[i]->name == name)
        {
            outputNodes[i]->updateVal(msg.getArgAsFloat(0));
            return;
        }
    }

    // create new output node
    addOutput(name, msg.getArgAsFloat(0));
}

void OscController::handleTwoValues(osc::Message &msg)
{
    std::string path = msg.getAddress();
    std::ostringstream names[2];
    names[0] = std::ostringstream();
    names[0] << path << " : x";
    names[1] = std::ostringstream();
    names[1] << path << " : y";
    
    for (int i=0; i<2; i++)
    {
        bool bExists = false;
        for (int n=0; n<outputNodes.size(); n++)
        {
            if (outputNodes[n]->name == names[i].str())
            {
                bExists = true;
                outputNodes[n]->updateVal(msg.getArgAsFloat(i));
            }
        }
        if (bExists) {
            continue;
        }
        
        // create new output node
        addOutput(names[i].str(), msg.getArgAsFloat(i));
    }
}

void OscController::addInput(osc::Message msg)
{
    ProgramInput *input = new ProgramInput();
    if (input->setup(oscSender, msg)) {
        inputNodes.push_back(new InputNode(inputNodes.size(), this, nextInputPos));
        nextInputPos.y += 15;
        inputs.push_back(input);
        resizeComponent();
    }
    else {
        delete input;
    }
}

void OscController::addOutput(std::string name, float val)
{
    OutputNode *node = new OutputNode(outputNodes.size(), this, nextOutputPos);
    node->setName(name);
    node->updateVal(val);
    outputNodes.push_back(node);
    nextOutputPos.y += 15;
    resizeComponent();
}

void OscController::handleOutputMessage(osc::Message msg)
{
    if (msg.getNumArgs() != 2) {
        return;
    }
    
    int index = msg.getArgAsInt32(0);
    float val = msg.getArgAsFloat(1);
    outputNodes[index]->updateVal(val);
}

void OscController::resizeComponent()
{
    int nodeNum = max(inputNodes.size(), outputNodes.size());
    canvasRect.y2 = canvasRect.y1 + 20 + nodeNum*15;
    localRect.y2 = 20 + nodeNum*15;
}

void printMessage(osc::Message message)
{
    console() << message.getAddress() << endl;
    
    for (int i = 0; i < message.getNumArgs(); i++) {
        console() << "-- Argument " << i << std::endl;
        console() << "---- type: " << message.getArgTypeName(i) << std::endl;
        if( message.getArgType(i) == osc::TYPE_INT32 ) {
            try {
                console() << "------ value: "<< message.getArgAsInt32(i) << std::endl;
            }
            catch (...) {
                console() << "Exception reading argument as int32" << std::endl;
            }
        }
        else if( message.getArgType(i) == osc::TYPE_FLOAT ) {
            try {
                console() << "------ value: " << message.getArgAsFloat(i) << std::endl;
            }
            catch (...) {
                console() << "Exception reading argument as float" << std::endl;
            }
        }
        else if( message.getArgType(i) == osc::TYPE_STRING) {
            try {
                console() << "------ value: " << message.getArgAsString(i).c_str() << std::endl;
            }
            catch (...) {
                console() << "Exception reading argument as string" << std::endl;
            }
        }
    }
}

