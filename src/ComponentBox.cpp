//
//  ComponentBox.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#include "ComponentBox.h"

ComponentBox::ComponentBox(Vec2f p, Vec2f s)
{
    rect = Rectf(p, p+s);
    
    createButtons();
}

void ComponentBox::update()
{
    
}

void ComponentBox::draw()
{
    gl::pushMatrices();
    gl::translate(rect.getUpperLeft());
    
    gl::color(1, 1, 1);
    gl::drawSolidRect(Rectf(Vec2f(0, 0), rect.getSize()));
    gl::color(0, 0, 0);
    gl::drawStrokedRect(Rectf(Vec2f(0, 0), rect.getSize()));
    
    for (int i=0; i<buttons.size(); i++)
    {
        buttons[i]->draw();
    }
    
    gl::popMatrices();
}

ComponentButton* ComponentBox::mouseDown( MouseEvent event )
{
    Vec2f local = getLocalCoords(event.getPos());
    for (int i=0; i<buttons.size(); i++)
    {
        if (buttons[i]->contains(local)) {
            return buttons[i];
        }
    }
    
    return NULL;
}

bool ComponentBox::contains(Vec2f p)
{
    return rect.contains(p);
}

void ComponentBox::setSize(Vec2f newSize)
{
    rect.x2 = rect.x1 + newSize.x;
    rect.y2 = rect.y1 + newSize.y;
}

void ComponentBox::createButtons()
{
    float startY = 10;
    buttons.push_back(new ComponentButton(COMPONENT_TYPE_PROGRAM, Vec2f(10, startY), Vec2f(rect.getWidth()-20, 20)));
    buttons.push_back(new ComponentButton(COMPONENT_TYPE_NUMBER, Vec2f(10, startY+25), Vec2f(rect.getWidth()-20, 20)));
    buttons.push_back(new ComponentButton(COMPONENT_TYPE_SPLIT, Vec2f(10, startY+50), Vec2f(rect.getWidth()-20, 20)));
    buttons.push_back(new ComponentButton(COMPONENT_TYPE_OSCILLATOR, Vec2f(10, startY+75), Vec2f(rect.getWidth()-20, 20)));
    buttons.push_back(new ComponentButton(COMPONENT_TYPE_EXP, Vec2f(10, startY+100), Vec2f(rect.getWidth()-20, 20)));
//    buttons.push_back(new ComponentButton(COMPONENT_TYPE_JS, Vec2f(10, startY+250), Vec2f(rect.getWidth()-20, 20)));
    
    createJSButtons(startY+125);
}

void ComponentBox::createJSButtons(float y)
{
    // scan components dir and look for .js files
    stringstream pathStr;
    pathStr << getAppPath().c_str()<<"/Contents/Resources/components/";
    fs::path path(pathStr.str());
    fs::directory_iterator end_iter;
    
    if (!fs::exists(path) || !fs::is_directory(path)) {
        return;
    }
    
    for (fs::directory_iterator file(path); file!=end_iter; ++file)
    {
        if (fs::is_regular_file(file->status())) {
            console() << "found file: "<<*file<<endl;
            buttons.push_back(new ComponentButton(COMPONENT_TYPE_JS, Vec2f(10, y), Vec2f(rect.getWidth()-20, 20), file->path().string()));
            y+=25;
        }
    }
    
    
}


Vec2f ComponentBox::getLocalCoords(Vec2f p)
{
    return p-rect.getUpperLeft();
}