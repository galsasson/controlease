//
//  ComponentFactory.cpp
//  Controlease
//
//  Created by Gal Sasson on 4/4/14.
//
//

#include "ComponentFactory.h"

CanvasComponent* ComponentFactory::newComponent(ComponentButton* button, Canvas* canvas, Vec2f p)
{
    if (button == NULL || canvas == NULL) {
        return NULL;
    }
    
    switch (button->type) {
        case COMPONENT_TYPE_PROGRAM:
            return new Program(canvas, p + Vec2f(30, 30));
        case COMPONENT_TYPE_NUMBER:
            return new ::Number(canvas, p + Vec2f(30, 30), Vec2f(100, 40));
        case COMPONENT_TYPE_SPLIT:
            return new Split(canvas, p + Vec2f(30, 30));
        case COMPONENT_TYPE_OSCILLATOR:
            return new Oscillator(canvas, p + Vec2f(30, 30), Vec2f(100, 40));
        case COMPONENT_TYPE_EXP:
            return new Exp(canvas, p + Vec2f(30, 30), Vec2f(250, 50));
        case COMPONENT_TYPE_JS:
            return new JSComponent(canvas, p + Vec2f(30, 30), button->source);
        case COMPONENT_TYPE_OSCCONTROLLER:
            return new OscController(canvas, p + Vec2f(30, 30));
    }
    
    return NULL;
}