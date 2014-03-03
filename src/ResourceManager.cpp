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
}

gl::TextureFontRef ResourceManager::getTextureFont()
{
    return mTextureFont;
}

