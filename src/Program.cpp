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
    pos = _pos;
    setSize(Vec2f(200, 20));
    nextInputPos = Vec2f(6, 28);
    
    isMouseDown = false;
    connected = false;
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
}

void Program::setupConnection(int oport, int iport)
{
    programPort = oport;
    listenPort = iport;
    
    connect();
}

void Program::update()
{
    // handle incoming messages from the program
    handleMessages();
    
    if (!connected) {
        return;
    }    
}

void Program::draw()
{
    if (!connected) {
        // draw a non active program
        gl::pushMatrices();
        gl::translate(pos);
        gl::color(1, 1, 1);
        gl::drawSolidRoundedRect(Rectf(0, 0, size.x, size.y), 3);
        gl::color(0, 0, 0);
        gl::drawStrokedRoundedRect(Rectf(0, 0, size.x, size.y), 3);
        gl::color(1, 0.1, 0.1);
        gl::drawLine(Vec2f(0, size.y/2), Vec2f(size.x, size.y/2));
        gl::popMatrices();
        return;
    }

    gl::pushMatrices();
    gl::translate(pos);

    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(Rectf(0, 0, size.x, size.y), 3);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(Rectf(0, 0, size.x, size.y), 3);
    ResourceManager::getInstance().getTextureFont()->drawString(name, Vec2f(size.x/2 - halfNameSize.x, 15));
    gl::drawLine(Vec2f(0, 20), Vec2f(size.x, 20));

    // draw all the input names
    for (int i=0; i<inputs.size(); i++)
    {
        ResourceManager::getInstance().getTextureFont()->drawString(inputs[i]->getName(), Vec2f(15, 30+i*15));
    }
    
    // draw all input nodes
    for (int i=0; i<inputNodes.size(); i++)
    {
        inputNodes[i]->draw();
    }
    
    
    
    gl::popMatrices();
}

void Program::drawOutline()
{
    gl::pushMatrices();
    gl::translate(pos-Vec2f(4, 4));
    
    glPushAttrib(GL_ENABLE_BIT);
    glLineStipple(1, 0xff00);
    gl::enable(GL_LINE_STIPPLE);

    gl::color(0, 0, 0);
    gl::drawStrokedRect(Rectf(Vec2f(0, 0), Vec2f(size.x+8, size.y+8)));
    
    glPopAttrib();
    gl::popMatrices();
}

void Program::mouseDown(cease::MouseEvent event)
{
    isMouseDown = true;
    prevMouse = event.getPos();
}

void Program::mouseUp(cease::MouseEvent event)
{
    isMouseDown = false;
}

void Program::mouseWheel(cease::MouseEvent event)
{
}

void Program::mouseMove(cease::MouseEvent event)
{
    
}

void Program::mouseDrag(cease::MouseEvent event)
{
    pos += event.getPos() - prevMouse;
    prevMouse = event.getPos();
    
    applyBorders();
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
    vector<Node*> outputs;
    
    return outputs;
}

bool Program::contains(Vec2f p)
{
    Area a = Area(pos, pos+size);
    return a.contains(p);
}

Vec2f Program::getCanvasPos()
{
    return pos;
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
    return p-pos;
}

Vec2f Program::getCanvasCoords(Vec2f p)
{
    return pos+p;
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
		
		console() << "New message received" << std::endl;
		console() << "Address: " << message.getAddress() << std::endl;
		console() << "Num Arg: " << message.getNumArgs() << std::endl;
        
        if (message.getAddress() == "/alive!") {
            handleAlive(message);
            return;
        }
        else if (message.getAddress() == "/input_param") {
            addInput(message);
            return;
        }
        else if (message.getAddress() == "/end_params") {
            if (inputs.size() > 0 && name != "") {
                connected = true;
            }
            return;
        }
        
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

void Program::handleAlive(osc::Message msg)
{
    if (msg.getNumArgs() != 1) {
        return;
    }
    
    name = msg.getArgAsString(0);
    nameSize = ResourceManager::getInstance().getTextureFont()->measureString(name);
    halfNameSize = nameSize/2;
}

void Program::addInput(osc::Message msg)
{
    ProgramInput *input = new ProgramInput();
    if (input->setup(oscSender, msg)) {
        inputNodes.push_back(new InputNode(inputNodes.size(), this, nextInputPos));
        setSize(size+Vec2f(0, 15));
        nextInputPos.y += 15;
        inputs.push_back(input);
    }
    else {
        delete input;
    }
}

void Program::setSize(Vec2f s)
{
    size = s;
    halfSize = s/2;
}

void Program::applyBorders()
{
    float x1 = pos.x;
    float x2 = pos.x + size.x;
    float y1 = pos.y;
    float y2 = pos.y + size.y;
    
    if (x1 < 0) {
        pos.x += -x1;
    }
    else if (x2 > CANVAS_WIDTH) {
        pos.x -= x2-CANVAS_WIDTH;
    }
    
    if (y1 < 0) {
        pos.y += -y1;
    }
    else if (y2 > CANVAS_HEIGHT) {
        pos.y -= y2-CANVAS_HEIGHT;
    }
}

