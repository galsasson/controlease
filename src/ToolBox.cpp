//
//  ToolBox.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "ToolBox.h"

ToolBox::ToolBox(Vec2f p, Vec2f s)
{
    rect = Rectf(p, p+s);
    
    createTools();
}

void ToolBox::update()
{
    
}

void ToolBox::draw()
{
    gl::pushMatrices();
    gl::translate(rect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRect(Rectf(Vec2f(0, 0), rect.getSize()));
    gl::color(0, 0, 0);
    gl::drawStrokedRect(Rectf(Vec2f(0, 0), rect.getSize()));
    
    for (int i=0; i<tools.size(); i++)
    {
        tools[i]->draw();
    }
    
    gl::popMatrices();
}

Tool* ToolBox::mouseDown( MouseEvent event )
{
    Vec2f local = getLocalCoords(event.getPos());
    for (int i=0; i<tools.size(); i++)
    {
        if (tools[i]->contains(local)) {
            return tools[i];
        }
    }
    
    return NULL;
}

bool ToolBox::contains(Vec2f p)
{
    return rect.contains(p);
}

void ToolBox::setSize(Vec2f newSize)
{
    rect.x2 = rect.x1 + newSize.x;
    rect.y2 = rect.y1 + newSize.y;
}

void ToolBox::createTools()
{
    float startY = 10;
    tools.push_back(new Tool(TOOL_TYPE_PROGRAM, Vec2f(10, startY), Vec2f(rect.getWidth()-20, 20)));
    tools.push_back(new Tool(TOOL_TYPE_NUMBER, Vec2f(10, startY+25), Vec2f(rect.getWidth()-20, 20)));
    tools.push_back(new Tool(TOOL_TYPE_SPLIT, Vec2f(10, startY+50), Vec2f(rect.getWidth()-20, 20)));
    tools.push_back(new Tool(TOOL_TYPE_ADD, Vec2f(10, startY+75), Vec2f(rect.getWidth()-20, 20)));
    tools.push_back(new Tool(TOOL_TYPE_SUB, Vec2f(10, startY+100), Vec2f(rect.getWidth()-20, 20)));
    tools.push_back(new Tool(TOOL_TYPE_MUL, Vec2f(10, startY+125), Vec2f(rect.getWidth()-20, 20)));
    tools.push_back(new Tool(TOOL_TYPE_DIV, Vec2f(10, startY+150), Vec2f(rect.getWidth()-20, 20)));
    tools.push_back(new Tool(TOOL_TYPE_RANDOM, Vec2f(10, startY+175), Vec2f(rect.getWidth()-20, 20)));
    tools.push_back(new Tool(TOOL_TYPE_OSCILLATOR, Vec2f(10, startY+200), Vec2f(rect.getWidth()-20, 20)));
    tools.push_back(new Tool(TOOL_TYPE_EXP, Vec2f(10, startY+225), Vec2f(rect.getWidth()-20, 20)));
}

Vec2f ToolBox::getLocalCoords(Vec2f p)
{
    return p-rect.getUpperLeft();
}