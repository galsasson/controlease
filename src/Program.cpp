//
//  Program.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#include "Program.h"

Program::Program(Vec2f _pos)
{
    titleRect = Rectf(5, 0, 200, 20);
    localRect = Rectf(0, 0, 200, 40);
    rect = Rectf(_pos, _pos+localRect.getSize());
    nextInputPos = Vec2f(6, 28);
    nextOutputPos = Vec2f(localRect.x2 - 6, 28);
    
    textInputRect = Rectf(7, 22, 200, 36);
    addressInput = new TextInput(Vec2f(7, 22), Vec2f(200, 14));
    addressInput->onReturn(boost::bind(&Program::addressInputSet, this));

    listenPort = Rand::randInt(5000, 9000);
    name = "Program";

    connected = false;
    isEditing = false;
}

Program::~Program()
{
    // delete all inputs
    for (int i=0; i<inputs.size(); i++)
    {
        delete inputs[i];
    }
    
    for (int i=0; i<inputNodes.size(); i++)
    {
        delete inputNodes[i];
    }
    
    delete addressInput;
}

void Program::setupConnection(string host, int oport)
{
    programHost = host;
    programPort = oport;
    
    connect();
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
    
    if (!connected && isEditing) {
        addressInput->update();
        return;
    }
}

void Program::draw()
{
    gl::pushMatrices();
    gl::translate(rect.getUpperLeft());

    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(localRect, 3);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(localRect, 3);

    ResourceManager::getInstance().getTextureFont()->drawString(name, titleRect);
    gl::drawLine(Vec2f(0, 20), Vec2f(localRect.x2, 20));

    if (connected) {
        // draw all the input names
        for (int i=0; i<inputs.size(); i++)
        {
            ResourceManager::getInstance().getTextureFont()->drawString(inputs[i]->getName(), Vec2f(15, 30+i*15));
        }
        for (int i=0; i<outputs.size(); i++)
        {
            outputs[i]->draw();
        }
    
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

void Program::drawOutline()
{
    gl::pushMatrices();
    gl::translate(rect.getUpperLeft());
    if (!connected)
    {
        addressInput->drawInFocus();
    }
    
    gl::translate(Vec2f(-4, -4));
    
    glPushAttrib(GL_ENABLE_BIT);
    glLineStipple(1, 0xff00);
    gl::enable(GL_LINE_STIPPLE);

    gl::color(0, 0, 0);
    gl::drawStrokedRect(Rectf(Vec2f(0, 0), rect.getSize() + Vec2f(8, 8)));
    
    glPopAttrib();
    
    gl::popMatrices();
}

void Program::translate(Vec2f offset)
{
    rect += offset;
}

Rectf Program::getBounds()
{
    return rect;
}

void Program::mouseDown(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    isEditing = false;
    
    if (!connected) {
        if (textInputRect.contains(local)) {
            isEditing = true;
            return;
        }
    }

    prevMouse = event.getPos();
}

void Program::mouseUp(cease::MouseEvent event)
{
}

void Program::mouseWheel(cease::MouseEvent event)
{
}

void Program::mouseMove(cease::MouseEvent event)
{
    
}

void Program::mouseDrag(cease::MouseEvent event)
{
    rect += event.getPos() - prevMouse;
    prevMouse = event.getPos();
    
    applyBorders();
}

bool Program::isHotspot(cease::MouseEvent event)
{
    Vec2f local = getLocalCoords(event.getPos());
    return titleRect.contains(local) || textInputRect.contains(local);
}

bool Program::isDragPoint(cease::MouseEvent event)
{
    return titleRect.contains(getLocalCoords(event.getPos()));
}

ConnectionResult* Program::getConnectionStart(cease::MouseEvent event)
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

ConnectionResult* Program::getConnectionEnd(cease::MouseEvent event)
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
    
    return NULL;
}

vector<Node*> Program::getInputNodes()
{
    vector<Node*> inodes;
    
    for (int i=0; i<inputNodes.size(); i++)
    {
        inodes.push_back((Node*)inputNodes[i]);
        
    }
    return inodes;
}

vector<Node*> Program::getOutputNodes()
{
    vector<Node*> onodes;
    
    for (int i=0; i<outputNodes.size(); i++)
    {
        onodes.push_back((Node*)outputNodes[i]);
    }
    
    return onodes;
}

bool Program::contains(Vec2f p)
{
    return rect.contains(p);
}

KeyboardListener* Program::getCurrentKeyboardListener()
{
    if (!connected && isEditing) {
        return addressInput;
    }
    
    return NULL;
}

Vec2f Program::getCanvasPos()
{
    return rect.getUpperLeft();
}

float Program::getValue(int i)
{
    return inputs[i]->getValue();
}

void Program::setValue(int i, float v)
{
    inputs[i]->sendVal(v);
}

Vec2f Program::getLocalCoords(Vec2f p)
{
    return p-rect.getUpperLeft();
}

Vec2f Program::getCanvasCoords(Vec2f p)
{
    return rect.getUpperLeft()+p;
}


void Program::connect()
{
    connected = false;
    boost::posix_time::ptime s = boost::posix_time::second_clock::local_time();
    
    oscSender = new osc::Sender();
    oscSender->setup("localhost", programPort);
    oscListener.setup(listenPort);
    
    osc::Message helloMsg;
    helloMsg.setAddress("/alive?");
    helloMsg.addStringArg("127.0.0.1");
    helloMsg.addIntArg(listenPort);
    oscSender->sendMessage(helloMsg);
    
    boost::posix_time::ptime e = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_duration duration = e-s;
    console() <<"Time to connect: "<<duration.total_milliseconds()<<endl;
}

void Program::handleMessages()
{
    while( oscListener.hasWaitingMessages() ) {
		osc::Message message;
		oscListener.getNextMessage( &message );
		
//		console() << "New message received" << std::endl;
//		console() << "Address: " << message.getAddress() << std::endl;
//		console() << "Num Arg: " << message.getNumArgs() << std::endl;
        
        if (message.getAddress() == "/oc") {
            handleOutputMessage(message);
            continue;
        }
        if (message.getAddress() == "/alive!") {
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
                connected = true;
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
    
    name = msg.getArgAsString(0);
    Vec2f nameSize = ResourceManager::getInstance().getTextureFont()->measureString(name);
    nameRect = Rectf(Vec2f(2, 2), Vec2f(2, 2) + nameSize);
//    halfNameSize = nameSize/2;
}

void Program::addInput(osc::Message msg)
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

void Program::addOutput(osc::Message msg)
{
    ProgramOutput *poutput = new ProgramOutput();
    if (poutput->setup(msg, nextOutputPos))
    {
        outputs.push_back(poutput);
        OutputNode *node = new OutputNode(poutput->getIndex(), this, nextOutputPos);
        node->updateVal(poutput->getValue());
        outputNodes.push_back(node);
        nextOutputPos.y += 15;
        resizeComponent();
    }
    else {
        delete poutput;
    }
}

void Program::handleOutputMessage(osc::Message msg)
{
    if (msg.getNumArgs() != 2) {
        return;
    }
    
    int index = msg.getArgAsInt32(0);
    float val = msg.getArgAsFloat(1);
    outputNodes[index]->updateVal(val);
}

void Program::applyBorders()
{
    float x1 = rect.getUpperLeft().x;
    float x2 = rect.getUpperRight().x;
    float y1 = rect.getUpperLeft().y;
    float y2 = rect.getLowerLeft().y;
    
    if (x1 < 0) {
        rect += Vec2f(-x1, 0);
    }
    else if (x2 > CANVAS_WIDTH) {
        rect -= Vec2f(x2-CANVAS_WIDTH, 0);
    }
    
    if (y1 < 0) {
        rect += Vec2f(0, -y1);
    }
    else if (y2 > CANVAS_HEIGHT) {
        rect -= Vec2f(0, y2-CANVAS_HEIGHT);
    }
}

void Program::resizeComponent()
{
    int nodeNum = max(inputNodes.size(), outputNodes.size());
    rect.y2 = rect.y1 + 20 + nodeNum*15;
    localRect.y2 = 20 + nodeNum*15;
}

