//
//  ComponentButton.h
//  Controlease
//
//  Created by Gal Sasson on 2/23/14.
//
//

#ifndef __Controlease__ComponentButton__
#define __Controlease__ComponentButton__

#include <iostream>

#include "ResourceManager.h"

using namespace ci;
using namespace std;

typedef enum _ComponentType {
    COMPONENT_TYPE_PROGRAM,
    COMPONENT_TYPE_NUMBER,
    COMPONENT_TYPE_SPLIT,
    COMPONENT_TYPE_OSCILLATOR,
    COMPONENT_TYPE_EXP,
    COMPONENT_TYPE_JS,
} ComponentType;

class ComponentButton
{
public:
    ComponentButton(ComponentType t, Vec2f pos, Vec2f size);
    ComponentButton(ComponentType t, Vec2f pos, Vec2f size, std::string src);
    ~ComponentButton();
    
    void draw();
    bool contains(Vec2f p);
    
    std::string getComponentName();

    ComponentType type;
    fs::path source;

private:
//    Vec2f getLocalCoords(Vec2f p);
    
    Rectf rect;
    Rectf localRect;
    string str;
    Rectf textRect;
};

#endif /* defined(__Controlease__ComponentButton__) */
