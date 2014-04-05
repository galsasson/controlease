//
//  MenuBar.h
//  Controlease
//
//  Created by Gal Sasson on 4/4/14.
//
//

#ifndef __Controlease__MenuBar__
#define __Controlease__MenuBar__

#include <iostream>
#include "ResourceManager.h"
#include "Button.h"

using namespace ci;
using namespace std;

class MenuBar : MouseListener
{
public:
    MenuBar(Vec2f _pos, Vec2f _size);
    ~MenuBar();
    
    void draw();
    
    void setSize(Vec2f size);
    
private:
    void initButtons();
    
    Rectf parentRect;
    Rectf localRect;
    
    vector<Button*> buttons;
};


#endif /* defined(__Controlease__MenuBar__) */
