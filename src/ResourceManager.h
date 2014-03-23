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

using namespace ci;
using namespace ci::app;
using namespace std;

class ResourceManager
{
public:
    static ResourceManager& getInstance()
    {
        static ResourceManager instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    
    void initResources();
    gl::TextureFontRef getTextureFont();
    Colorf getColor(int index);
private:
    ResourceManager() {};
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    ResourceManager(ResourceManager const&);              // Don't Implement
    void operator=(ResourceManager const&); // Don't implement
    
    Font mFont;
    gl::TextureFontRef mTextureFont;
    vector<Colorf> colors;
};
#endif /* defined(__tunnelvars__ResourceManager__) */
