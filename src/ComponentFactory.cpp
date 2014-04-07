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
            comp = new Program(canvas);
            break;
        case COMPONENT_TYPE_NUMBER:
            comp = new ::Number(canvas);
            break;
        case COMPONENT_TYPE_SPLIT:
            comp = new Split(canvas);
            break;
        case COMPONENT_TYPE_OSCILLATOR:
            comp = new Oscillator(canvas);
            break;
        case COMPONENT_TYPE_EXP:
            comp = new Exp(canvas);
            break;
        case COMPONENT_TYPE_JS:
        {
            JSComponent *jsComp = new JSComponent(canvas);
            jsComp->initNew(p + Vec2f(30, 30), button->source);
            return jsComp;
        }
        case COMPONENT_TYPE_OSCCONTROLLER:
            comp = new OscController(canvas);
            break;
        default:
            return NULL;
    }
    
    comp->initNew(p + Vec2f(30, 30));
    return comp;
}

CanvasComponent* ComponentFactory::newComponent(Canvas *canvas, const XmlTree& xml)
{
    CanvasComponent *comp = NULL;
    
    ComponentType type = (ComponentType)xml.getAttributeValue<int>("type");
    switch (type) {
        case COMPONENT_TYPE_PROGRAM:
            comp = new Program(canvas);
            break;
        case COMPONENT_TYPE_NUMBER:
            comp = new ::Number(canvas);
            break;
        case COMPONENT_TYPE_SPLIT:
            comp = new Split(canvas);
            break;
        case COMPONENT_TYPE_OSCILLATOR:
            comp = new Oscillator(canvas);
            break;
        case COMPONENT_TYPE_EXP:
            comp = new Exp(canvas);
            break;
        case COMPONENT_TYPE_JS:
        {
            JSComponent *jsComp = new JSComponent(canvas);
            jsComp->initFromXml(xml);
            return jsComp;
        }
        case COMPONENT_TYPE_OSCCONTROLLER:
            comp = new OscController(canvas);
            break;
        default:
            return NULL;
    }

    comp->initFromXml(xml);
    return comp;
    
    
}