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
    TOOL_TYPE_CONSTANT,
} ToolType;

class Tool
{
public:
    Tool(ToolType t, Vec2f pos, Vec2f size);
    
    void draw();
    bool contains(Vec2f p);
    
    static string getToolName(ToolType t);

    ToolType type;

private:
//    Vec2f getLocalCoords(Vec2f p);
    
    Rectf rect;
    Rectf localRect;
    string str;
    Rectf textRect;
};

#endif /* defined(__Controlease__Tool__) */
