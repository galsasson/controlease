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
#include "ResourceManager.h"
#include "Node.h"

using namespace std;
using namespace ci;
using namespace ci::app;


class ProgramInput : public InputNode
{
public:
    string name;
    string address;
    int index;
    ValueType type;
    int intVal;
    float floatVal;
    bool boolVal;
    float minVal;
    float maxVal;
    
    ProgramInput();
    bool setup(osc::Sender *sender, osc::Message msg, Vec2f p);
    void update();
    void draw();
    
    void updateVal(float val);
    
private:
    osc::Sender *oscSender;
    string getValueString();
    bool initialized;
    Vec2f pos;
};



#endif /* defined(__tunnelvars__ProgramInput__) */
