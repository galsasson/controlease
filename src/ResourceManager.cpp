//
//  ResourceManager.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#include "ResourceManager.h"

v8::Isolate* ResourceManager::mainIsolate;

void ResourceManager::initResources()
{
//    mFont = Font(loadAsset("Blender-BOOK.ttf"), 14);
//    mFont = Font(loadAsset("Blender-BOLD.ttf"), 14);
    mFont = Font(loadAsset("Overlock-Bold.ttf"), 12);
    gl::TextureFont::Format format;
//    format.premultiply();
    mTextureFont = gl::TextureFont::create(mFont, format);
    
    mPlusTexture = loadImage(getAssetPath("plus.png"));
    
    colors.push_back(Color::hex(0xdc534e));
    colors.push_back(Color::hex(0x5f78b4));
    colors.push_back(Color::hex(0x5b3a6f));
    colors.push_back(Color::hex(0xbaa326));
    colors.push_back(Color::hex(0xF26835));
    
    ResourceManager::mainIsolate = v8::Isolate::New();
    ResourceManager::mainIsolate->Enter();
}

gl::TextureFontRef ResourceManager::getTextureFont()
{
    return mTextureFont;
}

gl::Texture& ResourceManager::getPlusTexture()
{
    return mPlusTexture;
}

Colorf ResourceManager::getColor(int index)
{
    if (index < 0 || index>=colors.size())
    {
        return Colorf(0, 0, 0);
    }
    
    return colors[index];
}
