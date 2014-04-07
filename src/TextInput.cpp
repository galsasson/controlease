//
//  TextInput.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/28/14.
//
//

#include "TextInput.h"

TextInput::TextInput(Vec2f p, Vec2f s, bool multiline)
{
    canvasRect = Rectf(p, p+s);
    localRect = Rectf(0, 0, s.x, s.y);
    isMultiline = multiline;
    
    blinkCounter = 0;
    
    str = "";
    fontHeight = ResourceManager::getInstance().getTextureFont()->getAscent() +
    ResourceManager::getInstance().getTextureFont()->getDescent();

    cursorPos = 0;
    cursorLocation = Vec2f(0,0);
    strRect = Rectf(0, 0, 0, fontHeight);
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
        gl::color(0, 0, 0);
        gl::drawLine(cursorLocation, cursorLocation + Vec2f(0, fontHeight));
    }
    
    gl::popMatrices();
}

void TextInput::keyDown(cinder::app::KeyEvent event)
{
//    console() << event.getCode() << endl;
    
    if (event.getCode() == event.KEY_RETURN) {
        if (isMultiline && event.isShiftDown()) {
            // SHIFT enter: do enter
            str.insert(cursorPos, 1, event.getChar());
            cursorPos++;
        }
        else if (!returnFunction.empty()) {
            // handle ENTER
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
    strRect = Rectf(0, 0, strSize.x, strSize.y);

    cursorLocation = getCursorLocation();
    updateSize();
}

void TextInput::keyUp(cinder::app::KeyEvent event)
{
    
}

void TextInput::onReturn(boost::function<void(void)> func)
{
    returnFunction = func;
}

int TextInput::getNumLines()
{
    int lines=1;
    for (int i=0; i<str.length(); i++)
    {
        if (str[i] == 13) {
            lines++;
        }
    }
    
    return lines;
}

Vec2f TextInput::getTextSize()
{
    return localRect.getSize();
}

int TextInput::getLineStart(const std::string& text, int pos)
{
    if (pos==0) {
        return 0;
    }
    int index = text.rfind(13, pos-1);
    
    if (index < 0) {
        return 0;
    }
    return index+1;
}

std::string TextInput::getLineUntil(const std::string& text, int pos)
{
    int start = getLineStart(text, pos);
    return text.substr(start, pos-start);
}

int TextInput::getLineIndex(const std::string& text, int pos)
{
    int lines=0;
    for (int i=0; i<pos; i++)
    {
        if (text[i] == 13) {
            lines++;
        }
    }
    return lines;
}

Vec2f TextInput::getCursorLocation()
{
    Vec2f loc(0, 0);
    
    std::string line = getLineUntil(str, cursorPos);
    
    loc.x = ResourceManager::getInstance().getTextureFont()->measureString(line).x;
    loc.y = (float)getLineIndex(str, cursorPos)*fontHeight;
    
    if (loc.y < 0) {
        loc.y = 0;
    }
    
    return loc;
}

void TextInput::updateSize()
{
    localRect = strRect;
    localRect.y2 = getNumLines()*fontHeight;
    canvasRect.x2 = localRect.x2;
    canvasRect.y2 = localRect.y2;
}




