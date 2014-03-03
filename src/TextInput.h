//
//  TextInput.h
//  Controlease
//
//  Created by Gal Sasson on 2/28/14.
//
//

#ifndef __Controlease__TextInput__
#define __Controlease__TextInput__

#include <iostream>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "MouseListener.h"
#include "KeyboardListener.h"
#include "ResourceManager.h"
#include "CanvasComponent.h"

using namespace ci;
using namespace ci::gl;
using namespace std;


class TextInput : public KeyboardListener
{
public:
    TextInput(Vec2f p, Vec2f s);
    
    void update();
    void draw();
    void drawInFocus();
    
    void keyDown( KeyEvent event );
    void keyUp( KeyEvent event );
    
    void onReturn(boost::function<void(void)> func);

    string getValue() { return str; };
    
private:
    Rectf canvasRect;
    Rectf localRect;
    string str;
    Rectf strRect;
    
    int cursorPos;
    float cursorX;
    
    int blinkCounter;
    
    boost::function<void(void)> returnFunction;
    
    
    
};

#endif /* defined(__Controlease__TextInput__) */
