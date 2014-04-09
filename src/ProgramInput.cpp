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
    minVal = maxVal = 0;
}

bool ProgramInput::initNew(osc::Sender *sender, osc::Message msg)
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
            boolVal = !!msg.getArgAsFloat(4);
        }
        else if (type == TYPE_INT32) {
            intVal = (int)msg.getArgAsFloat(4);
        }
        else if (type == TYPE_FLOAT) {
            floatVal = msg.getArgAsFloat(4);
        }
    }
    catch(...) {
        return false;
    }
    
    initialized = true;
    return true;
}

void ProgramInput::initFromXml(osc::Sender *sender, const cinder::XmlTree& xml)
{
    oscSender = sender;
    
    name = xml.getAttributeValue<std::string>("name");
    address = xml.getAttributeValue<std::string>("address");
    index = xml.getAttributeValue<int>("index");
    type = (ValueType)xml.getAttributeValue<int>("type");
    if (type == ValueType::TYPE_BOOLEAN) {
        boolVal = xml.getAttributeValue<bool>("value");
    }
    else if (type == ValueType::TYPE_INT32) {
        intVal = xml.getAttributeValue<int>("value");
    }
    else if (type == ValueType::TYPE_FLOAT) {
        floatVal = xml.getAttributeValue<float>("value");
    }
    
    initialized = true;
}

XmlTree ProgramInput::getXml()
{
    XmlTree xml("ProgramInput", "");
    xml.setAttribute("name", name);
    xml.setAttribute("address", address);
    xml.setAttribute("index", index);
    xml.setAttribute("type", type);
    
    if (type == ValueType::TYPE_BOOLEAN) {
        xml.setAttribute("value", boolVal);
    }
    else if (type == ValueType::TYPE_INT32) {
        xml.setAttribute("value", intVal);
    }
    else if (type == ValueType::TYPE_FLOAT) {
        xml.setAttribute("value", floatVal);
    }
    
    return xml;
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
        msg.addFloatArg(intVal);
        oscSender->sendMessage(msg);
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
        msg.addFloatArg(boolVal);
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



