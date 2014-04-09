//
//  ProgramOutput.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#include "ProgramOutput.h"

ProgramOutput::ProgramOutput()
{
}

bool ProgramOutput::initNew(osc::Message msg)
{
    if (msg.getNumArgs() < 4) {
        return false;
    }
    
    name = msg.getArgAsString(0);
    index = msg.getArgAsInt32(1);
    type = (ValueType)msg.getArgAsInt32(2);
    value = msg.getArgAsFloat(3);
    
    return true;
}

void ProgramOutput::initFromXml(const cinder::XmlTree &xml)
{
    name = xml.getAttributeValue<std::string>("name");
    index = xml.getAttributeValue<int>("index");
    type = (ValueType)xml.getAttributeValue<int>("type");
    value = xml.getAttributeValue<float>("value");
}

XmlTree ProgramOutput::getXml()
{
    XmlTree xml("ProgramOutput", "");
    
    xml.setAttribute("name", name);
    xml.setAttribute("index", index);
    xml.setAttribute("type", type);
    xml.setAttribute("value", value);
    
    return xml;
}