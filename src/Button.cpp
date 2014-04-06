//
//  Button.cpp
//  Controlease
//
//  Created by Gal Sasson on 4/4/14.
//
//

#include "Button.h"

Button::Button(std::string _text, Vec2f _pos, Vec2f _size)
{
    text = _text;
    parentRect = Rectf(_pos, _pos+_size);
    localRect = Rectf(Vec2f(0, 0), _size);
    
    Vec2f textSize = ResourceManager::getInstance().getTextureFont()->measureString(text);
    textRect = Rectf(_size/2 - textSize/2, _size/2 + textSize/2);
}

void Button::draw()
{
    gl::pushMatrices();
    gl::translate(parentRect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRoundedRect(localRect, 3);
    gl::color(0, 0, 0);
    gl::drawStrokedRoundedRect(localRect, 3);
    
    ResourceManager::getInstance().getTextureFont()->drawString(text, textRect);
    
    gl::popMatrices();
}

bool Button::contains(Vec2f p)
{
    return parentRect.contains(p);
}

void Button::click()
{
    signal(this);
}