//
//  ResourceManager.cpp
//  tunnelvars
//
//  Created by Gal Sasson on 2/19/14.
//
//

#include "ResourceManager.h"

void ResourceManager::initResources()
{
    mFont = Font("Arial", 12);
    mTextureFont = gl::TextureFont::create(mFont);
}

gl::TextureFontRef ResourceManager::getTextureFont()
{
    return mTextureFont;
}

