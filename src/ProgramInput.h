//
//  ProgramInput.h
//  tunnelvars
//
//  Created by Gal Sasson on 2/19/14.
//
//

#ifndef __tunnelvars__ProgramInput__
#define __tunnelvars__ProgramInput__

#include <iostream>
#include "OscListener.h"
#include "ResourceManager.h"

using namespace std;
using namespace ci;
using namespace ci::app;


class ProgramInput
{
public:
    typedef enum _InputType
    {
        TYPE_INT32 = 1,
        TYPE_FLOAT = 2,
        TYPE_BOOLEAN = 3
    } InputType;

    string name;
    string address;
    int index;
    int type;
    int intVal;
    float floatVal;
    bool boolVal;
    float minVal;
    float maxVal;
    
    ProgramInput();
    bool setup(osc::Message msg, Vec2f p);
    void update();
    void draw();
    
private:
    bool initialized;
    Vec2f pos;
};



#endif /* defined(__tunnelvars__ProgramInput__) */
