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
    pos = p;
    size = s;
}

void ToolBox::update()
{
    
}

void ToolBox::draw()
{
    gl::pushMatrices();
    gl::translate(pos);
    
    gl::color(1, 1, 1);
    gl::drawSolidRect(Rectf(pos, pos+size));
    gl::color(0, 0, 0);
    gl::drawStrokedRect(Rectf(pos, pos+size));
    
    gl::popMatrices();
}