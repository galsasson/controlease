//
//  Program.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#include "Program.h"

Program::Program(Canvas *c) : CanvasComponent(c)
{
    setType(ComponentType::COMPONENT_TYPE_PROGRAM);
    bConnected = false;
    bEditing = false;
    bSenderRecieverInitialized = false;
    oscSender = new osc::Sender();
    bRunThread = true;
	
#ifdef ALWAYS_UPDATE
    outputThread = new std::thread(&Program::outputThreadWorker, this);
#endif
}

Program::~Program()
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
    
#ifdef ALWAYS_UPDATE
    bRunThread = false;
    outputThread->join();
    delete outputThread;
#endif
    
    delete addressInput;
}

void Program::initNew(Vec2f pos)
{
    CanvasComponent::initNew(pos);
    setSize(Vec2f(200, 40));
    pushNodesDown(17);

    addressInput = new TextInput();
    addressInput->initNew(Vec2f(4, 23), Vec2f(192, 14));
    addressInput->onReturn(boost::bind(&Program::addressInputSet, this));
    
    listenPort = Rand::randInt(5000, 9000);
}

void Program::initFromXml(const XmlTree& xml, bool createNodes)
{
    CanvasComponent::initFromXml(xml);
    
    addressInput = new TextInput();
    addressInput->initFromXml(xml.getChild("TextInput"));
    addressInput->onReturn(boost::bind(&Program::addressInputSet, this));
    
    listenPort = xml.getAttributeValue<int>("listenPort");
    bConnected = xml.getAttributeValue<bool>("bConnected");
    bEditing = xml.getAttributeValue<bool>("bEditing");
    
    if (bConnected) {
        programHost = xml.getAttributeValue<std::string>("programHost");
        programPort = xml.getAttributeValue<int>("programPort");
        
        // create program inputs
        XmlTree programInputsXml = xml.getChild("ProgramInputs");
        for(XmlTree::ConstIter iter = programInputsXml.begin(); iter != programInputsXml.end(); ++iter)
        {
            if (iter->getTag() == "ProgramInput") {
                ProgramInput *newInput = new ProgramInput();
                newInput->initFromXml(oscSender, iter->getChild(""));
                inputs.push_back(newInput);
            }
        }
        
        // create program outputs
        XmlTree programOutputsXml = xml.getChild("ProgramOutputs");
        for(XmlTree::ConstIter iter = programOutputsXml.begin(); iter != programOutputsXml.end(); ++iter)
        {
            if (iter->getTag() == "ProgramOutput") {
                ProgramOutput *newOutput = new ProgramOutput();
                newOutput->initFromXml(iter->getChild(""));
                outputs.push_back(newOutput);
            }
        }
        
        setupConnection(programHost, programPort);
//        
//        if (!createSenderListener()) {
//            return;
//        }
//        
//        // send the program the hello message to update it on our
//        // current ip and port number
//        sendHelloMessage();
    }
    
}

XmlTree Program::getXml()
{
    XmlTree xml = CanvasComponent::getXml();
 
    xml.push_back(addressInput->getXml());
    xml.setAttribute("listenPort", listenPort);
    xml.setAttribute("bConnected", bConnected);
    xml.setAttribute("bEditing", bEditing);
    
    if (bConnected) {
        xml.setAttribute("programHost", programHost);
        xml.setAttribute("programPort", programPort);
        
        // save all program inputs
        XmlTree pInputs("ProgramInputs", "");
        for (int i=0; i<inputs.size(); i++)
        {
            pInputs.push_back(inputs[i]->getXml());
        }
        xml.push_back(pInputs);
        
        // save all program outputs
        XmlTree pOutputs("ProgramOutputs", "");
        for (int i=0; i<outputs.size(); i++)
        {
            pOutputs.push_back(outputs[i]->getXml());
        }
        xml.push_back(pOutputs);        
    }
    return xml;
}


void Program::setupConnection(string host, int oport)
{
    bConnected = false;
    
    programHost = host;
    programPort = oport;
    
    if (!createSenderListener()) {
        return;
    }
    
    sendHelloMessage();
    sendAliveMessage();
}

void Program::addressInputSet(void)
{
    string textVal = addressInput->getValue();
    
    string host;
    int port;
    
    int collonIndex = textVal.find(':');
    if (collonIndex < 0) {
        return;
    }
    
    host = textVal.substr(0, collonIndex);
    port = atoi(textVal.substr(collonIndex+1, textVal.size()).c_str());
    
    console() << "connecting to host: " << host << ", port "<<port<<endl;
    
    setupConnection(host, port);
}

void Program::update()
{
    // handle incoming messages from the program
    handleMessages();
    
    if (bEditing) {
        addressInput->update();
    }
}

void Program::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());

    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(localRect, 3);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(localRect, 3);

    gl::lineWidth(1);
    ResourceManager::getInstance().getTextureFont()->drawString(name, titleRect);
    gl::color(0, 0, 0);
    gl::drawLine(Vec2f(2, titleRect.y2), Vec2f(localRect.getWidth()-4, titleRect.y2));

    if (bConnected) {
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
    
    addressInput->draw();
    
    gl::popMatrices();
}

void Program::drawOutline()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());

    addressInput->drawInFocus();
    
    gl::translate(Vec2f(-4, -4));
    
    glPushAttrib(GL_ENABLE_BIT);
    glLineStipple(1, 0xff00);
    gl::enable(GL_LINE_STIPPLE);

    gl::color(0.5, 0.5, 0.5);
    gl::drawStrokedRect(Rectf(Vec2f(0, 0), canvasRect.getSize() + Vec2f(8, 8)));
    
    gl::disable(GL_LINE_STIPPLE);
    glPopAttrib();
    
    gl::popMatrices();
}

void Program::mouseDown(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    bEditing = false;
    
    if (addressInput->contains(local)) {
        bEditing = true;
        return;
    }

    prevMouse = event.getPos();
}

void Program::mouseDrag(const cease::MouseEvent& event)
{
    canvasRect += event.getPos() - prevMouse;
    prevMouse = event.getPos();
    
    applyBorders();
}

bool Program::isHotspot(const cease::MouseEvent& event)
{
    Vec2f local = toLocal(event.getPos());
    return titleRect.contains(local) || (addressInput->contains(local));
}

bool Program::isDragPoint(const cease::MouseEvent& event)
{
    return titleRect.contains(toLocal(event.getPos()));
}

KeyboardListener* Program::getCurrentKeyboardListener()
{
    if (bEditing) {
        return addressInput;
    }
    
    return NULL;
}

float Program::getValue(int i)
{
    return outputs[i]->getValue();
}

void Program::setValue(int i, float v)
{
    if (!bSenderRecieverInitialized) {
        return;
    }
    
#ifndef ALWAYS_UPDATE
    inputs[i]->sendVal(v);
#endif
}

bool Program::createSenderListener()
{
    try {
        oscSender->setup(programHost, programPort);
        oscListener.setup(listenPort);
        bSenderRecieverInitialized = true;
    }
    catch (...)
    {
        console() << "error creating oscsender and listener"<<endl;
        return false;
    }
    
    return true;
}

/* sendHelloMessage will tell the remote program
   my ip and my listen port
 */
void Program::sendHelloMessage()
{
    if (!bSenderRecieverInitialized) {
        return;
    }
    
    osc::Message helloMsg;
    helloMsg.setAddress("/hello");
    helloMsg.addIntArg(listenPort);
    oscSender->sendMessage(helloMsg);
}

/* sendAliveMessage will tell the remote program to
   send me all its inputs and outputs in a following 
   '/alive!' message */
void Program::sendAliveMessage()
{
    if (!bSenderRecieverInitialized) {
        return;
    }
    
    osc::Message aliveMsg;
    aliveMsg.setAddress("/alive?");
    oscSender->sendMessage(aliveMsg);
}

void Program::handleMessages()
{
    if (!bSenderRecieverInitialized) {
        return;
    }

    while( oscListener.hasWaitingMessages() ) {
		osc::Message message;
		oscListener.getNextMessage( &message );
		
        if (message.getAddress() == "/oc") {
            handleOutputMessage(message);
            continue;
        }
        else if (message.getAddress() == "/alive!") {
            handleAlive(message);
            continue;
        }
        else if (message.getAddress() == "/input_node") {
            addInput(message);
            continue;
        }
        else if (message.getAddress() == "/output_node") {
            addOutput(message);
            continue;
        }
        else if (message.getAddress() == "/end_nodes") {
            if (inputs.size() > 0 && name != "") {
                bConnected = true;
            }
            continue;
        }
        else {
            // just print the message
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
    }
}

void Program::handleAlive(osc::Message msg)
{
    if (msg.getNumArgs() != 1) {
        return;
    }
    
    setName(msg.getArgAsString(0));
}

void Program::addInput(osc::Message msg)
{
    // TODO: don't use ProgramInput
    ProgramInput *pinput = new ProgramInput();
    if (pinput->initNew(oscSender, msg)) {
        // if we already have input with this name, don't do anything
        if (doesInputNameExists(pinput->getName())) {
            delete pinput;
            return;
        }
        
        inputs.push_back(pinput);
        InputNode *node = addNewInputNode();
        node->setOriginalVal(pinput->getValue());
        node->setName(pinput->getName());
        node->bDisplayName = true;
        pack(0, 0);
    }
    else {
        delete pinput;
    }
}

void Program::addOutput(osc::Message msg)
{
    ProgramOutput *poutput = new ProgramOutput();
    if (poutput->initNew(msg))
    {
        // if we already have input with this name, don't do anything
        if (doesOutputNameExists(poutput->getName())) {
            delete poutput;
            return;
        }
        
        outputs.push_back(poutput);
        OutputNode *node = addNewOutputNode();
        node->updateVal(poutput->getValue());
        node->setName(poutput->getName());
        node->bDisplayName = true;
        pack(0, 0);
    }
    else {
        delete poutput;
    }
}

void Program::handleOutputMessage(osc::Message& msg)
{
    if (msg.getNumArgs() != 2) {
        return;
    }
    
    int index = msg.getArgAsInt32(0);
    float val = msg.getArgAsFloat(1);
    outputNodes[index]->updateVal(val);
}

bool Program::doesInputNameExists(std::string name)
{
    for (int i=0; i<inputNodes.size(); i++)
    {
        if (inputNodes[i]->getName() == name) {
            return true;
        }
    }
    
    return false;
}

bool Program::doesOutputNameExists(std::string name)
{
    for (int i=0; i<outputNodes.size(); i++)
    {
        if (outputNodes[i]->getName() == name) {
            return true;
        }
    }
    
    return false;
}

void Program::sendAllInputs()
{
    osc::Message msg;
    msg.setAddress("/all");
    for (int i=0; i<inputNodes.size(); i++)
    {
        msg.addFloatArg(inputNodes[i]->getLastVal());
    }
    oscSender->sendMessage(msg);
}

void Program::outputThreadWorker()
{
    console() << "Program worker thread started"<<endl;
    while (bRunThread)
    {
        if (bConnected)
        {
            sendAllInputs();
            usleep(16666);
        }
        else {
            usleep(50000);
        }
    }
    console() << "Program worker thread finished"<<endl;
}

