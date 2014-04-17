//
//  OscController.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#include "OscController.h"

void printMessage(osc::Message message);

OscController::OscController(Canvas *c) : CanvasComponent(c)
{
    setType(ComponentType::COMPONENT_TYPE_OSCCONTROLLER);
    
    bEditing = false;
    bConnected = false;
}

OscController::~OscController()
{
    if (bConnected) {
        bConnected = false;
        handleMsg.join();
    }
    
    delete addressInput;
}

void OscController::initNew(Vec2f pos)
{
    CanvasComponent::initNew(pos);
    setSize(Vec2f(100, 40));
    
    addressInput = new TextInput();
    addressInput->initNew(Vec2f(5, 23), Vec2f(90, 14));
    addressInput->onReturn(boost::bind(&OscController::addressInputSet, this));
}

void OscController::initFromXml(const XmlTree& xml, bool createNodes)
{
    CanvasComponent::initFromXml(xml);
    
    addressInput = new TextInput();
    addressInput->initFromXml(xml.getChild("TextInput"));
    addressInput->onReturn(boost::bind(&OscController::addressInputSet, this));

    bConnected = xml.getAttributeValue<bool>("bConnected");
    if (bConnected) {
        listenPort = xml.getAttributeValue<int>("listenPort");
        setupConnection(listenPort);
    }
}

XmlTree OscController::getXml()
{
    XmlTree xml = CanvasComponent::getXml();
    
    xml.setAttribute("bConnected", bConnected);
    xml.setAttribute("listenPort", listenPort);
    xml.push_back(addressInput->getXml());
    
    return xml;
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

void OscController::setupConnection(int port)
{
    listenPort = port;
    
    startListener();
}

void OscController::update()
{
    if (!bConnected && bEditing) {
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
    gl::drawLine(Vec2f(2, titleRect.y2), Vec2f(localRect.x2-4, titleRect.y2));

    if (bConnected)
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
    if (!bConnected)
    {
        addressInput->drawInFocus();
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

void OscController::mouseDown(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    bEditing = false;
    
    if (!bConnected) {
        if (addressInput->contains(local)) {
            bEditing = true;
            return;
        }
    }

    prevMouse = event.getPos();
}

void OscController::mouseDrag(const cease::MouseEvent& event)
{
    canvasRect += event.getPos() - prevMouse;
    prevMouse = event.getPos();
    
    applyBorders();
}

bool OscController::isHotspot(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    return titleRect.contains(local) || (!bConnected && addressInput->contains(local));
}

bool OscController::isDragPoint(const cease::MouseEvent& event)
{
    return titleRect.contains(toLocal(event.getPos()));
}

KeyboardListener* OscController::getCurrentKeyboardListener()
{
    if (!bConnected && bEditing) {
        return addressInput;
    }
    
    return NULL;
}

float OscController::getValue(int i)
{
    return outputNodes[i]->getLastVal();
}

void OscController::setValue(int i, float v)
{
    console() << "warning OscController::setValue is not implemented"<<endl;
//    inputs[i]->sendVal(v);
}

void OscController::startListener()
{
    oscListener.setup(listenPort);
    // handle incoming messages from the OscController
    bConnected = true;
//    int id = oscListener.registerMessageReceived(this, &OscController::handleMessage);
    handleMsg = std::thread(&OscController::handleMessages, this);
}

void OscController::handleMessages()
{
    while (bConnected)
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
        usleep(1000);
    }
}

void OscController::handleOneValue(osc::Message &msg)
{
    std::string name = msg.getAddress();
    float val = 0;
    if (msg.getArgType(0) == osc::ArgType::TYPE_FLOAT) {
        val = msg.getArgAsFloat(0);
    }
    else if (msg.getArgType(0) == osc::ArgType::TYPE_INT32) {
        val = msg.getArgAsInt32(0);
    }
    
    for (int i=0; i<outputNodes.size(); i++)
    {
        if (outputNodes[i]->getName() == name)
        {
            outputNodes[i]->updateVal(val);
            return;
        }
    }

    // create new output node
    addOutput(name, val);
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
            if (outputNodes[n]->getName() == names[i].str())
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

void OscController::addOutput(std::string name, float val)
{
    OutputNode* node = addNewOutputNode();
    node->setName(name);
    node->updateVal(val);
    node->bDisplayName = true;

    pack(0, 0);
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

