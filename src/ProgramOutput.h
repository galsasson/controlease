//
//  ProgramOutput.h
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#ifndef __Controlease__ProgramOutput__
#define __Controlease__ProgramOutput__

#include <iostream>
#include <sstream>

#include "cinder/Xml.h"

#include "OscListener.h"
#include "OscSender.h"

#include "Controlease.h"
#include "ResourceManager.h"

using namespace std;
using namespace ci;

class ProgramOutput
{
public:
    ProgramOutput();
    bool initNew(osc::Message msg);
    void initFromXml(const XmlTree& xml);
    XmlTree getXml();
    
    int getIndex() { return index; }
    string getName() { return name; }
    float getValue() { return value; }
private:
    
    string name;
    int index;
    ValueType type;
    float value;
};



#endif /* defined(__tunnelvars__ProgramOutput__) */
