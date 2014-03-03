//
//  V8Engine.h
//  Controlease
//
//  Created by Gal Sasson on 3/1/14.
//
//

#ifndef __Controlease__V8Engine__
#define __Controlease__V8Engine__

#include <iostream>
#include "v8.h"

using namespace std;
using namespace v8;

class V8Engine
{
public:
    static V8Engine& getInstance()
    {
        static V8Engine instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    
    void initEngine();
    Handle<Script> getScript(string sourceStr);
private:
    V8Engine() {};
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    V8Engine(V8Engine const&);              // Don't Implement
    void operator=(V8Engine const&); // Don't implement
    
    
    Isolate* isolate;
};

#endif /* defined(__Controlease__V8Engine__) */
