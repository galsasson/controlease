//
//  KeyboardListener.h
//  Controlease
//
//  Created by Gal Sasson on 2/28/14.
//
//

#ifndef Controlease_KeyboardListener_h
#define Controlease_KeyboardListener_h

#include "cinder/app/Event.h"

using namespace ci::app;

class KeyboardListener
{
public:
    virtual void keyDown(KeyEvent event) = 0;
    virtual void keyUp(KeyEvent event) = 0;
};


#endif
