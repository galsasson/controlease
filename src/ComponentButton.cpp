//
//  ComponentButton.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/23/14.
//
//

#include "ComponentButton.h"

ComponentButton::ComponentButton(ComponentType t, Vec2f pos, Vec2f size)
{
    type = t;
    rect = Rectf(pos, pos+size);
    localRect = Rectf(Vec2f(0, 0), size);
    
    str = CanvasComponent::getComponentTypeString(t);
    Vec2f strSize = ResourceManager::getInstance().getTextureFont()->measureString(str);
    textRect = Rectf(size/2 - strSize/2, size/2 + strSize/2);
}

ComponentButton::ComponentButton(ComponentType t, Vec2f pos, Vec2f size, std::string src)
{
    type = t;
    rect = Rectf(pos, pos+size);
    localRect = Rectf(Vec2f(0, 0), size);
    
    source = fs::path(src);
    str = CanvasComponent::getJSComponentTypeString(src);
    Vec2f strSize = ResourceManager::getInstance().getTextureFont()->measureString(str);
    textRect = Rectf(size/2 - strSize/2, size/2 + strSize/2);
}

ComponentButton::~ComponentButton()
{
}

void ComponentButton::draw()
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

bool ComponentButton::contains(Vec2f p)
{
    return rect.contains(p);
}
