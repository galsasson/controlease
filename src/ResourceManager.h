//
//  ResourceManager.h
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#ifndef __Controlease__ResourceManager__
#define __Controlease__ResourceManager__

#include <iostream>
#include <vector>
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/TextureFont.h"
#include "v8.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ResourceManager
{
public:
    static ResourceManager& getInstance()
    {
        static ResourceManager instance;
        // Instantiated on first use.
        return instance;
    }
    
    void initResources();
    
    gl::TextureFontRef getTextureFont();
    Colorf getColor(int index);
    gl::Texture& getPlusTexture();

    static v8::Isolate* mainIsolate;

private:
    ResourceManager() {};

    // make sure there are no copies
    ResourceManager(ResourceManager const&);
    void operator=(ResourceManager const&);
    
    Font mFont;
    gl::TextureFontRef mTextureFont;
    gl::Texture mPlusTexture;
    vector<Colorf> colors;
    
};
#endif /* defined(__tunnelvars__ResourceManager__) */
