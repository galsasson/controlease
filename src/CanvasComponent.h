//
//  Component.h
//  Controlease
//
//  Created by Gal Sasson on 2/22/14.
//
//

#ifndef __Controlease__Component__
#define __Controlease__Component__

#include "MouseListener.h"

class CanvasComponent : public MouseListener
{
public:
    virtual ~CanvasComponent() {};
    virtual void update() = 0;
    virtual void draw() = 0;
};


#endif /* defined(__Controlease__Component__) */
