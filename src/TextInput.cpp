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
    
    ResourceManager::getInstance().getTextureFont()->drawString(str, strRect);
    
    gl::popMatrices();
}

void TextInput::drawInFocus()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());

    // draw blinking cursor
    if (blinkCounter > 10) {
        gl::drawLine(strRect.getUpperRight(), strRect.getLowerRight());
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
        if (str.length() > 0) {
            str.erase(str.length()-1, str.length());
        }
    }
    else if (event.getCode() >= 32 && event.getCode() <= 126) {
        str.append(1, event.getChar());
    }
    
    Vec2f strSize = ResourceManager::getInstance().getTextureFont()->measureString(str);
    strRect = Rectf(0, 0, strSize.x, localRect.getHeight());
}

void TextInput::keyUp(cinder::app::KeyEvent event)
{
    
}

void TextInput::onReturn(boost::function<void(void)> func)
{
    returnFunction = func;
}

