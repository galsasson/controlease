//
//  Tool.h
//  Controlease
//
//  Created by Gal Sasson on 2/23/14.
//
//

#ifndef __Controlease__Tool__
#define __Controlease__Tool__

#include <iostream>

#include "ResourceManager.h"

using namespace ci;
using namespace std;

typedef enum _ToolType {
    TOOL_TYPE_PROGRAM,
    TOOL_TYPE_NUMBER,
    TOOL_TYPE_SPLIT,
    TOOL_TYPE_ADD,
    TOOL_TYPE_SUB,
    TOOL_TYPE_MUL,
    TOOL_TYPE_DIV,
    TOOL_TYPE_NOISE,
    TOOL_TYPE_RANDOM,
    TOOL_TYPE_OSCILLATOR,
    TOOL_TYPE_EXP,
    TOOL_TYPE_JS,
} ToolType;

class Tool
{
public:
    Tool(ToolType t, Vec2f pos, Vec2f size);
    Tool(ToolType t, Vec2f pos, Vec2f size, string src);
    
    void draw();
    bool contains(Vec2f p);
    
    static string getToolName(ToolType t);

    ToolType type;
    string source;

private:
//    Vec2f getLocalCoords(Vec2f p);
    
    Rectf rect;
    Rectf localRect;
    string str;
    Rectf textRect;
};

#endif /* defined(__Controlease__Tool__) */
