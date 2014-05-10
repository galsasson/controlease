//
//  ProgramInput.h
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#ifndef __Controlease__ProgramInput__
#define __Controlease__ProgramInput__

#include <iostream>
#include <sstream>
#include "cinder/Xml.h"
#include "OscListener.h"
#include "OscSender.h"

#include "Controlease.h"

using namespace std;
using namespace ci;

class ProgramInput
{
public:
    ProgramInput();
    bool initNew(osc::Sender *sender, osc::Message msg);
    void initFromXml(osc::Sender *sender, const XmlTree& xml);
    XmlTree getXml();

    string getName() { return name; }
    int getIndex() { return index; }
    void setIndex(int i) { index = i; }
    float getValue();
    
    void sendVal(float val);

private:
    string getValueString();
    
    osc::Sender *oscSender;
    bool initialized;

    string name;
    string address;
    int index;
    ValueType type;
    int intVal;
    float floatVal;
    bool boolVal;
    float minVal;
    float maxVal;
};



#endif /* defined(__tunnelvars__ProgramInput__) */
