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

void ProgramOutput::draw()
{
    gl::pushMatrices();
    gl::translate(pos);
    
    ResourceManager::getInstance().getTextureFont()->drawString(name, nameRect);
    
    gl::popMatrices();
    
}

bool ProgramOutput::setup(osc::Message msg, Vec2f p)
{
    pos = p;
    
    if (msg.getNumArgs() < 4) {
        return false;
    }
    
    name = msg.getArgAsString(0);
    index = msg.getArgAsInt32(1);
    type = (ValueType)msg.getArgAsInt32(2);
    value = msg.getArgAsFloat(3);
    
    Vec2f nameSize = ResourceManager::getInstance().getTextureFont()->measureString(name);
    nameRect = Rectf(-nameSize.x - 5, -nameSize.y/2-1, nameSize.x, nameSize.y);
    

    return true;
}



