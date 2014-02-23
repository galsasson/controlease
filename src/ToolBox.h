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

#include "MouseListener.h"

using namespace ci;
using namespace ci::gl;

class ToolBox : public MouseListener
{
public:
    ToolBox(Vec2f p, Vec2f s);
    void update();
    void draw();
    
private:
    Vec2f pos;
    Vec2f size;
};

#endif /* defined(__Controlease__ToolBox__) */
