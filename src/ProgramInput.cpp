//
//  ProgramInput.cpp
//  tunnelvars
//
//  Created by Gal Sasson on 2/19/14.
//
//

#include "ProgramInput.h"

ProgramInput::ProgramInput()
{
    initialized = false;
}

bool ProgramInput::setup(osc::Message msg, Vec2f p)
{
    pos = p;
    try {
        if (msg.getNumArgs() < 5) {
            return false;
        }
        
        name = msg.getArgAsString(0);
        address = msg.getArgAsString(1);
        index = msg.getArgAsInt32(2);
        type = msg.getArgAsInt32(3);
        if (type == ProgramInput::TYPE_BOOLEAN) {
            boolVal = !!msg.getArgAsInt32(4);
        }
        else if (type == ProgramInput::TYPE_INT32) {
            intVal = msg.getArgAsInt32(4);
            minVal = msg.getArgAsInt32(5);
            maxVal = msg.getArgAsInt32(6);
        }
        else if (type == ProgramInput::TYPE_FLOAT) {
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
    
    gl::color(0, 0, 0);
    gl::drawStrokedCircle(Vec2f(0, 0), 5);
    ResourceManager::getInstance().getTextureFont()->drawString(name, Vec2f(15, 3));
    
    gl::popMatrices();
}



