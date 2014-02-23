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
#include "OscListener.h"
#include "OscSender.h"

#include "Controlease.h"

using namespace std;
using namespace ci;

class ProgramInput
{
public:
    ProgramInput();
    bool setup(osc::Sender *sender, osc::Message msg);

    string getName();
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
