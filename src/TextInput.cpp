//
//  TextInput.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/28/14.
//
//

#include "TextInput.h"

TextInput::TextInput()
{
    blinkCounter = 0;
    
    text = "";
    fontHeight = ResourceManager::getInstance().getTextureFont()->getAscent() +
    ResourceManager::getInstance().getTextureFont()->getDescent();

    cursorPos = 0;
    cursorLocation = Vec2f(0,0);
    textRect = Rectf(0, 0, 0, fontHeight);
}

void TextInput::initNew(Vec2f p, Vec2f mSize, bool multiline)
{
    minSize = mSize;
    canvasRect = Rectf(p, p+minSize);
    localRect = Rectf(0, 0, minSize.x, minSize.y);
    bMultiline = multiline;
}

void TextInput::initFromXml(const cinder::XmlTree &xml)
{
    // TODO: implement
    Vec2f pos = Vec2f(xml.getAttributeValue<float>("pos.x"),
                      xml.getAttributeValue<float>("pos.y"));
    minSize = Vec2f(xml.getAttributeValue<float>("minSize.x"),
                    xml.getAttributeValue<float>("minSize.y"));
    canvasRect = Rectf(pos, pos+minSize);
    localRect = Rectf(Vec2f(0, 0), minSize);
    bMultiline = xml.getAttributeValue<bool>("bMultiline");
    text = xml.getAttributeValue<std::string>("text");
    updateTextSize();
}

XmlTree TextInput::getXml()
{
    XmlTree xml("TextInput", "");
    
    xml.setAttribute("pos.x", canvasRect.x1);
    xml.setAttribute("pos.y", canvasRect.y1);
    xml.setAttribute("minSize.x", minSize.x);
    xml.setAttribute("minSize.y", minSize.y);
    xml.setAttribute("bMultiline", bMultiline);
    xml.setAttribute("text", text);
    
    return xml;
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
    gl::lineWidth(0.5f);
    gl::drawStrokedRect(localRect);
    gl::lineWidth(1);
    ResourceManager::getInstance().getTextureFont()->drawString(text, textRect);
    
    gl::popMatrices();
}

void TextInput::drawInFocus()
{
    gl::pushMatrices();
    gl::translate(canvasRect.getUpperLeft());

    // draw blinking cursor
    if (blinkCounter > 10) {
        gl::color(0, 0, 0);
        gl::lineWidth(2);
        gl::drawLine(cursorLocation, cursorLocation + Vec2f(0, fontHeight));
        gl::lineWidth(1);
    }
    
    gl::popMatrices();
}

void TextInput::keyDown(cinder::app::KeyEvent event)
{
//    console() << event.getCode() << endl;
    
    if (event.getCode() == event.KEY_RETURN) {
        if (bMultiline && event.isShiftDown()) {
            // SHIFT enter: do enter
            text.insert(cursorPos, 1, event.getChar());
            cursorPos++;
        }
        else if (!returnFunction.empty()) {
            // handle ENTER
            returnFunction();
        }
    }
    else if (event.getCode() == event.KEY_BACKSPACE) {
        if (text.length() > 0 && cursorPos > 0) {
            text.erase(cursorPos-1, 1);
            cursorPos--;
        }
    }
    else if (event.getCode() == event.KEY_LEFT) {
        if (cursorPos > 0) {
            cursorPos--;
        }
    }
    else if (event.getCode() == event.KEY_RIGHT) {
        if (cursorPos < text.length()) {
            cursorPos++;
        }
    }
    else if (event.getCode() == event.KEY_KP_MINUS) {
        text.insert(cursorPos, 1, event.getChar());
        cursorPos++;
    }
    else if (event.getCode() >= 32 && event.getCode() <= 126) {
        text.insert(cursorPos, 1, event.getChar());
        cursorPos++;
    }
    
    updateTextSize();

    cursorLocation = getCursorLocation();
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
    for (int i=0; i<text.length(); i++)
    {
        if (text[i] == 13) {
            lines++;
        }
    }
    
    return lines;
}

Vec2f TextInput::getTextSize()
{
    return localRect.getSize();
}

bool TextInput::contains(Vec2f p)
{
    return canvasRect.contains(p);
}

int TextInput::getLineStart(const std::string& str, int pos)
{
    if (pos==0) {
        return 0;
    }
    int index = str.rfind(13, pos-1);
    
    if (index < 0) {
        return 0;
    }
    return index+1;
}

std::string TextInput::getLineUntil(const std::string& str, int pos)
{
    int start = getLineStart(str, pos);
    return str.substr(start, pos-start);
}

int TextInput::getLineIndex(const std::string& str, int pos)
{
    int lines=0;
    for (int i=0; i<pos; i++)
    {
        if (str[i] == 13) {
            lines++;
        }
    }
    return lines;
}

void TextInput::updateTextSize()
{
    Vec2f textSize = ResourceManager::getInstance().getTextureFont()->measureString(text);
    textRect = Rectf(0, 0, textSize.x, textSize.y);
    
    updateSize();
}

Vec2f TextInput::getCursorLocation()
{
    Vec2f loc(0, 0);
    
    std::string line = getLineUntil(text, cursorPos);
    
    loc.x = ResourceManager::getInstance().getTextureFont()->measureString(line).x;
    loc.y = (float)getLineIndex(text, cursorPos)*fontHeight;
    
    if (loc.y < 0) {
        loc.y = 0;
    }
    
    return loc;
}

void TextInput::updateSize()
{
    Vec2f size = Vec2f(textRect.getWidth(), textRect.getHeight());
    if (size.x < minSize.x) {
        size.x = minSize.x;
    }
    if (size.y < minSize.y) {
        size.y = minSize.y;
    }
    
    localRect.x2 = localRect.x1 + size.x;
    localRect.y2 = localRect.y1 + getNumLines()*fontHeight;
    canvasRect.x2 = canvasRect.x1 + localRect.getWidth();
    canvasRect.y2 = canvasRect.y1 + localRect.getHeight();
}




