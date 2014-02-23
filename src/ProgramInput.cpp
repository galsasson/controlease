//
//  ProgramInput.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#include "ProgramInput.h"

ProgramInput::ProgramInput()
{
    initialized = false;
}

bool ProgramInput::setup(osc::Sender *sender, osc::Message msg, Vec2f p)
{
    oscSender = sender;
    pos = p;
    try {
        if (msg.getNumArgs() < 5) {
            return false;
        }
        
        name = msg.getArgAsString(0);
        address = msg.getArgAsString(1);
        index = msg.getArgAsInt32(2);
        type = (ValueType)msg.getArgAsInt32(3);
        if (type == TYPE_BOOLEAN) {
            boolVal = !!msg.getArgAsInt32(4);
        }
        else if (type == TYPE_INT32) {
            intVal = msg.getArgAsInt32(4);
            minVal = msg.getArgAsInt32(5);
            maxVal = msg.getArgAsInt32(6);
        }
        else if (type == TYPE_FLOAT) {
            floatVal = msg.getArgAsFloat(4);
            minVal = msg.getArgAsFloat(5);
            maxVal = msg.getArgAsFloat(6);
        }
    }
    catch(...) {
        return false;
    }
    
    initialized = true;
    return true;
}

void ProgramInput::update()
{
    if (!initialized) {
        return;
    }
}

void ProgramInput::draw()
{
    if (!initialized) {
        return;
    }
    
    gl::pushMatrices();
    gl::translate(pos);
    gl::color(1, 1, 1);
    gl::drawSolidCircle(Vec2f(0, 0), 3);
    gl::color(0, 0, 0);
    gl::drawStrokedCircle(Vec2f(0, 0), 3);
    ResourceManager::getInstance().getTextureFont()->drawString(name, Vec2f(8, 2));
    
    gl::popMatrices();
}

void ProgramInput::updateVal(float val)
{
    if (type == TYPE_INT32) {
        intVal = (int)val;
        osc::Message msg;
        msg.setAddress(address);
        msg.addIntArg(index);
        msg.addIntArg(intVal);
        oscSender->sendMessage(msg);
//        console() << "updating value to: " << intVal<<endl;
    }
    else if (type == TYPE_FLOAT) {
        floatVal = val;
        osc::Message msg;
        msg.setAddress(address);
        msg.addIntArg(index);
        msg.addFloatArg(floatVal);
        oscSender->sendMessage(msg);
    }
    else if (type == TYPE_BOOLEAN) {
        boolVal = !!val;
        osc::Message msg;
        msg.setAddress(address);
        msg.addIntArg(index);
        msg.addIntArg((int)boolVal);
        oscSender->sendMessage(msg);
    }
}

string ProgramInput::getValueString()
{
    if (type == TYPE_INT32) {
        std::ostringstream str;
        str << intVal;
        return str.str();
    }
    else if (type == TYPE_FLOAT) {
        std::ostringstream str;
        str << floatVal << setprecision(4);
        return str.str();
    }
    else if (type == TYPE_BOOLEAN) {
        if (boolVal) {
            return "true";
        }
        else {
            return "false";
        }
    }
    else {
        return "-";
    }
}



