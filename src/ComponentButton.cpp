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
    
    str = getComponentName();
    Vec2f strSize = ResourceManager::getInstance().getTextureFont()->measureString(str);
    textRect = Rectf(size/2 - strSize/2, size/2 + strSize/2);
}

ComponentButton::ComponentButton(ComponentType t, Vec2f pos, Vec2f size, std::string src) : ComponentButton(t, pos, size)
{
    console()<<"ComponentButton("<<src<<")"<<endl;
    source = fs::path(src);
    str = getComponentName();
    Vec2f strSize = ResourceManager::getInstance().getTextureFont()->measureString(str);
    textRect = Rectf(size/2 - strSize/2, size/2 + strSize/2);
}

ComponentButton::~ComponentButton()
{
//    if (source) {
//        delete source;
//    }
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

string ComponentButton::getComponentName()
{
    switch (type)
    {
        case COMPONENT_TYPE_PROGRAM:
            return "Program";
        case COMPONENT_TYPE_NUMBER:
            return "Number";
        case COMPONENT_TYPE_SPLIT:
            return "Split";
        case COMPONENT_TYPE_OSCILLATOR:
            return "Oscillator";
        case COMPONENT_TYPE_EXP:
            return "Expression";
        case COMPONENT_TYPE_JS:
        {
            fs::path fileNoExt(source);
            fileNoExt = fileNoExt.replace_extension("").filename();
            console() << "getName = "<<fileNoExt.string()<<endl;
            return fileNoExt.string();
        }
        case COMPONENT_TYPE_OSCCONTROLLER:
            return "OscController";
    }
    
    return "Unknown";
}
