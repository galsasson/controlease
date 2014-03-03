//
//  Tool.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/23/14.
//
//

#include "Tool.h"

Tool::Tool(ToolType t, Vec2f pos, Vec2f size)
{
    type = t;
    rect = Rectf(pos, pos+size);
    localRect = Rectf(Vec2f(0, 0), size);
    
    str = Tool::getToolName(type);
    Vec2f strSize = ResourceManager::getInstance().getTextureFont()->measureString(str);
    textRect = Rectf(size/2 - strSize/2, size/2 + strSize/2);
}

void Tool::draw()
{
    gl::pushMatrices();
    gl::translate(rect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(localRect, 3);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(localRect, 3);

    ResourceManager::getInstance().getTextureFont()->drawString(str, textRect);
    
    gl::popMatrices();
}

bool Tool::contains(Vec2f p)
{
    return rect.contains(p);
}

string Tool::getToolName(ToolType t)
{
    switch (t)
    {
        case TOOL_TYPE_PROGRAM:
            return "Program";
        case TOOL_TYPE_NUMBER:
            return "Number";
        case TOOL_TYPE_SPLIT:
            return "Split";
        case TOOL_TYPE_ADD:
            return "Add";
        case TOOL_TYPE_SUB:
            return "Substract";
        case TOOL_TYPE_MUL:
            return "Multiply";
        case TOOL_TYPE_DIV:
            return "Divide";
        case TOOL_TYPE_NOISE:
            return "Noise";
        case TOOL_TYPE_RANDOM:
            return "Random";
        case TOOL_TYPE_OSCILLATOR:
            return "Oscillator";
        case TOOL_TYPE_EXP:
            return "Expression";
    }
    
    return "Unknown";
}
