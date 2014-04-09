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
    TextInput();
    
    void initNew(Vec2f p, Vec2f minSize, bool multiline = false);
    void initFromXml(const XmlTree& xml);
    XmlTree getXml();
    
    void update();
    void draw();
    void drawInFocus();
    
    void keyDown( KeyEvent event );
    void keyUp( KeyEvent event );
    
    void onReturn(boost::function<void(void)> func);

    int getNumLines();
    std::string& getValue() { return text; };
    static std::string getLineUntil(const std::string& str, int pos);
    static int getLineStart(const std::string& str, int pos);
    static int getLineIndex(const std::string& str, int pos);
    
    Vec2f getTextSize();
    
    bool contains(Vec2f p);

private:
    void updateTextSize();
    Vec2f getCursorLocation();
    void updateSize();
    
    Vec2f minSize;
    Rectf canvasRect;
    Rectf localRect;
    string text;
    Rectf textRect;
    
    bool bMultiline;
    
    int cursorPos;
    Vec2f cursorLocation;
    float fontHeight;
    
    int blinkCounter;
    
    boost::function<void(void)> returnFunction;
    
    
    
};

#endif /* defined(__Controlease__TextInput__) */
