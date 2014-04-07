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
    
    CanvasComponent* comp = NULL;
    
    switch (button->type) {
        case COMPONENT_TYPE_PROGRAM:
            comp = new Program(canvas, p + Vec2f(30, 30));
            break;
        case COMPONENT_TYPE_NUMBER:
            comp = new ::Number(canvas, p + Vec2f(30, 30));
            break;
        case COMPONENT_TYPE_SPLIT:
            comp = new Split(canvas, p + Vec2f(30, 30));
            break;
        case COMPONENT_TYPE_OSCILLATOR:
            comp = new Oscillator(canvas, p + Vec2f(30, 30));
            break;
        case COMPONENT_TYPE_EXP:
            comp = new Exp(canvas, p + Vec2f(30, 30));
            break;
        case COMPONENT_TYPE_JS:
            comp = new JSComponent(canvas, p + Vec2f(30, 30), button->source);
            break;
        case COMPONENT_TYPE_OSCCONTROLLER:
            comp = new OscController(canvas, p + Vec2f(30, 30));
            break;
        default:
            return NULL;
    }
    
    comp->initNew();
    return comp;
}