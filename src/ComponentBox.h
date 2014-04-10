//
//  ComponentBox.h
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#ifndef __Controlease__ComponentBox__
#define __Controlease__ComponentBox__

#include <iostream>
#include <sstream>
#include "Controlease.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

#include <vector>
#include "MouseListener.h"
#include "ComponentButton.h"

using namespace ci;
using namespace ci::gl;
using namespace std;

class ComponentBox
{
public:
    ComponentBox(Vec2f p, Vec2f s);
    void update();
    void draw();
    
    ComponentButton* mouseDown( MouseEvent event );
    bool contains(Vec2f p);

    void setSize(Vec2f newSize);
    
private:
    void createButtons();
    void createJSButtons(float y);
    Vec2f getLocalCoords(Vec2f p);
    
    vector<ComponentButton*> buttons;
    
    Rectf rect;
};

#endif /* defined(__Controlease__ComponentBox__) */
