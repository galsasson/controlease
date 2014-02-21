//
//  Program.cpp
//  tunnelvars
//
//  Created by Gal Sasson on 2/19/14.
//
//

#include "Program.h"

Program::Program(int oport, int iport, Vec2f _pos)
{
    programPort = oport;
    listenPort = iport;
    
    pos = _pos;
    setSize(Vec2f(200, 20));
    nextInputPos = Vec2f(-10, 30);
    
    connect();
}

Program::~Program()
{
    // delete all inputs
    for (int i=0; i<inputs.size(); i++)
    {
        delete inputs[i];
    }
    inputs.clear();
}

void Program::update()
{
    // handle incoming messages from the program
    handleMessages();
    
    if (!connected) {
        return;
    }
    
    
    for (int i=0; i<inputs.size(); i++)
    {
        inputs[i]->update();
    }
}

void Program::draw()
{
    if (!connected) {
        // draw a non active program
        gl::pushMatrices();
        gl::translate(pos);
        gl::color(0, 0, 0);
        gl::drawStrokedCircle(Vec2f(0, 0), 10);
        gl::color(1, 0.1, 0.1);
        gl::drawLine(Vec2f(-9, 0), Vec2f(9, 0));
        gl::popMatrices();
        return;
    }

    gl::pushMatrices();
    
    gl::translate(pos);

    gl::color(0, 0, 0);
    gl::drawStrokedRect(Rectf(0, 0, size.x, size.y));
    ResourceManager::getInstance().getTextureFont()->drawString(name, Vec2f(5, 15));
    gl::drawLine(Vec2f(0, 20), Vec2f(size.x, 20));

    // draw all the inputs
    for (int i=0; i<inputs.size(); i++)
    {
        inputs[i]->draw();
    }
    
    
    gl::popMatrices();
}

void Program::connect()
{
    connected = false;
    boost::posix_time::ptime s = boost::posix_time::second_clock::local_time();
    
    oscSender.setup("localhost", programPort);
    oscListener.setup(listenPort);
    
    osc::Message helloMsg;
    helloMsg.setAddress("/alive?");
    helloMsg.addStringArg("127.0.0.1");
    helloMsg.addIntArg(listenPort);
    oscSender.sendMessage(helloMsg);
    
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
    if (input->setup(msg, nextInputPos)) {
        setSize(size+Vec2f(0, 20));
        nextInputPos.y += 20;
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
