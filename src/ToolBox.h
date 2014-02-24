//
//  ToolBox.h
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#ifndef __Controlease__ToolBox__
#define __Controlease__ToolBox__

#include <iostream>
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

#include <vector>
#include "MouseListener.h"
#include "Tool.h"

using namespace ci;
using namespace ci::gl;
using namespace std;

class ToolBox
{
public:
    ToolBox(Vec2f p, Vec2f s);
    void update();
    void draw();
    
    Tool* mouseDown( MouseEvent event );
    bool contains(Vec2f p);

    void setSize(Vec2f newSize);
    
private:
    void createTools();
    Vec2f getLocalCoords(Vec2f p);
    
    vector<Tool*> tools;
    
    Rectf rect;
};

#endif /* defined(__Controlease__ToolBox__) */
