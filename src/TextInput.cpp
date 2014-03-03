//
//  TextInput.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/28/14.
//
//

#include "TextInput.h"

TextInput::TextInput(Vec2f p, Vec2f s)
{
    canvasRect = Rectf(p, p+s);
    localRect = Rectf(0, 0, s.x, s.y);
    
    blinkCounter = 0;
    
    str = "";
    strRect = Rectf(0, 0, 0, s.y);
    
    cursorPos = 0;
    cursorX = 0;
}

void TextInput::update()
{
    blinkCounter++;
    if (blinkCounter > 20) {
        blinkCounter = 0;
    }
}

void TextInput::draw()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());
    
    gl::color(0, 0, 0);
    ResourceManager::getInstance().getTextureFont()->drawString(str, strRect);
    
    gl::popMatrices();
}

void TextInput::drawInFocus()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());

    // draw blinking cursor
    if (blinkCounter > 10) {
        gl::drawLine(Vec2f(cursorX, 0), Vec2f(cursorX, strRect.getHeight()));
    }
    
    gl::popMatrices();
}

void TextInput::keyDown(cinder::app::KeyEvent event)
{
//    console() << event.getCode() << endl;
    
    if (event.getCode() == event.KEY_RETURN) {
        // handle ENTER
        if (!returnFunction.empty()) {
            returnFunction();
        }
    }
    else if (event.getCode() == event.KEY_BACKSPACE) {
        if (str.length() > 0 && cursorPos > 0) {
            str.erase(cursorPos-1, 1);
            cursorPos--;
        }
    }
    else if (event.getCode() == event.KEY_LEFT) {
        if (cursorPos > 0) {
            cursorPos--;
        }
    }
    else if (event.getCode() == event.KEY_RIGHT) {
        if (cursorPos < str.length()) {
            cursorPos++;
        }
    }
    else if (event.getCode() == event.KEY_KP_MINUS) {
        str.insert(cursorPos, 1, event.getChar());
        cursorPos++;
    }
    else if (event.getCode() >= 32 && event.getCode() <= 126) {
        str.insert(cursorPos, 1, event.getChar());
        cursorPos++;
    }
    
    Vec2f strSize = ResourceManager::getInstance().getTextureFont()->measureString(str);
    strRect = Rectf(0, 0, strSize.x, localRect.getHeight());
    cursorX = ResourceManager::getInstance().getTextureFont()->measureString(str.substr(0, cursorPos)).x;
}

void TextInput::keyUp(cinder::app::KeyEvent event)
{
    
}

void TextInput::onReturn(boost::function<void(void)> func)
{
    returnFunction = func;
}

