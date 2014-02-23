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
//    mFont = Font(loadAsset("Blender-BOOK.ttf"));
    mTextureFont = gl::TextureFont::create(Font(loadAsset("Blender-BOLD.ttf"), 14));
}

gl::TextureFontRef ResourceManager::getTextureFont()
{
    return mTextureFont;
}

