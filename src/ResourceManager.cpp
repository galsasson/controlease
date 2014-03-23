//
//  ResourceManager.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#include "ResourceManager.h"

void ResourceManager::initResources()
{
//    mFont = Font("Arial", 12);
//    mFont = Font(loadAsset("Blender-BOOK.ttf"), 14);
    mFont = Font(loadAsset("Blender-BOLD.ttf"), 14);
    mTextureFont = gl::TextureFont::create(mFont);
    
    colors.push_back(Color::hex(0x334d5c));
    colors.push_back(Color::hex(0x45b29d));
    colors.push_back(Color::hex(0xefc94c));
    colors.push_back(Color::hex(0xe27a3f));
    colors.push_back(Color::hex(0xdf5a49));
}

gl::TextureFontRef ResourceManager::getTextureFont()
{
    return mTextureFont;
}

Colorf ResourceManager::getColor(int index)
{
    if (index < 0 || index>=colors.size())
    {
        return Colorf(0, 0, 0);
    }
    
    return colors[index];
}
