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
    TextInput(Vec2f p, Vec2f s, bool multiline = false);
    
    void update();
    void draw();
    void drawInFocus();
    
    void keyDown( KeyEvent event );
    void keyUp( KeyEvent event );
    
    void onReturn(boost::function<void(void)> func);

    int getNumLines();
    string getValue() { return str; };
    static std::string getLineUntil(std::string, int pos);
    static int getLineStart(std::string text, int pos);
    static int getLineIndex(std::string text, int pos);
    
    Vec2f getTextSize();

private:
    void setCursorLocation();
    void updateSize();
    
    Rectf canvasRect;
    Rectf localRect;
    string str;
    Rectf strRect;
    
    bool isMultiline;
    
    int cursorLine;
    int cursorPos;
    Vec2f cursorLocation;
    float fontHeight;
    
    int blinkCounter;
    
    boost::function<void(void)> returnFunction;
    
    
    
};

#endif /* defined(__Controlease__TextInput__) */
