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

bool ProgramInput::setup(osc::Sender *sender, osc::Message msg)
{
    oscSender = sender;
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

string ProgramInput::getName()
{
    return name;
}

float ProgramInput::getValue()
{
    if (type == TYPE_INT32) {
        return (float)intVal;
    }
    else if (type == TYPE_FLOAT) {
        return floatVal;
    }
    else if (type == TYPE_BOOLEAN) {
        if (boolVal) {
            return 1;
        }
        else {
            return 0;
        }
    }
    
    return 0;
}

void ProgramInput::sendVal(float val)
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



