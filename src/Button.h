//
//  Button.h
//  Controlease
//
//  Created by Gal Sasson on 4/4/14.
//
//

#ifndef __Controlease__Button__
#define __Controlease__Button__

#include <iostream>
#include "cinder/app/App.h"
#include "ResourceManager.h"
#include "MouseListener.h"

using namespace ci;
using namespace std;

class Button
{
public:
    typedef boost::signals2::signal<void(Button*)> ButtonSignal;
    
    Button(std::string _text, Vec2f _pos, Vec2f _size);
    void draw();
    bool contains(Vec2f p);
    
    void click();
    ButtonSignal* getSignal() { return &signal; }

    std::string text;

private:
    Rectf parentRect;
    Rectf localRect;
    Rectf textRect;
        
    ButtonSignal signal;
};

#endif /* defined(__Controlease__Button__) */
