//
//  ComponentFactory.h
//  Controlease
//
//  Created by Gal Sasson on 4/4/14.
//
//

#ifndef __Controlease__ComponentFactory__
#define __Controlease__ComponentFactory__

#include <iostream>
#include "cinder/app/App.h"

#include "Controlease.h"
#include "CanvasComponent.h"
#include "ComponentButton.h"
#include "Program.h"
#include "Number.h"
#include "Split.h"
#include "Oscillator.h"
#include "Exp.h"
#include "JSComponent.h"
#include "OscController.h"

using namespace ci;

class Canvas;

class ComponentFactory
{
public:
    static CanvasComponent* newComponent(ComponentButton* button, Canvas* canvas, Vec2f p);
    static CanvasComponent* newComponent(Canvas *canvas, const XmlTree& xml);
};

#endif /* defined(__Controlease__ComponentFactory__) */
