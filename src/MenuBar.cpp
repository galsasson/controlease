//
//  MenuBar.cpp
//  Controlease
//
//  Created by Gal Sasson on 4/4/14.
//
//

#include "MenuBar.h"

MenuBar::MenuBar(Vec2f _pos, Vec2f _size)
{
    parentRect = Rectf(_pos, _pos+_size);
    localRect = Rectf(Vec2f(0, 0), _size);
    
    initButtons();
}

MenuBar::~MenuBar()
{
    for (int i=0; i<buttons.size(); i++)
    {
        delete buttons[i];
    }
}

void MenuBar::draw()
{
    gl::pushMatrices();
    gl::translate(parentRect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRect(localRect);
    gl::color(0, 0, 0);
    gl::drawStrokedRect(localRect);
    
    for (int i=0; i<buttons.size(); i++)
    {
        buttons[i]->draw();
    }
        
    gl::popMatrices();
}

void MenuBar::mouseDown(const cinder::app::MouseEvent& event)
{
    for (int i=0; i<buttons.size(); i++)
    {
        if (buttons[i]->contains(event.getPos())) {
            buttons[i]->click();
        }
    }
}

bool MenuBar::contains(const Vec2f& p)
{
    return parentRect.contains(p);
}

void MenuBar::onButtonClicked(boost::function<void(Button*)> func)
{
    for (int i=0; i<buttons.size(); i++)
    {
        buttons[i]->getSignal()->connect(func);
    }
}

void MenuBar::setSize(Vec2f size)
{
    parentRect.x2 = parentRect.x1 + size.x;
    parentRect.y2 = parentRect.y1 + size.y;
    localRect = Rectf(Vec2f(0, 0), size);
}

void MenuBar::initButtons()
{
    Vec2f bPos(5, 10);
    Vec2f bSize(50, 20);
    buttons.push_back(new Button("New", bPos, bSize));
    bPos.x += bSize.x+5;
    buttons.push_back(new Button("Open", bPos, bSize));
    bPos.x += bSize.x+5;
    buttons.push_back(new Button("Save", bPos, bSize));
}