//
//  V8Engine.cpp
//  Controlease
//
//  Created by Gal Sasson on 3/1/14.
//
//

#include "V8Engine.h"

void V8Engine::initEngine()
{
    // Get the default Isolate created at startup.
//    isolate = Isolate::GetCurrent();
//    
//    // Create a stack-allocated handle scope.
//    HandleScope handle_scope(isolate);
//    
////    // Create a new context.
//    Handle<Context> context = Context::New(isolate);
//    
//    Persistent<Context> persistent_context(isolate, context);
////
////    // Enter the context for compiling and running the hello world script.
//    Context::Scope context_scope(context);
//
//    // Create a string containing the JavaScript source code.
//    Handle<String> source = String::NewFromUtf8(isolate, "Math.sin(Math.PI/3)");
//    
//    // Compile the source code.
//    Handle<Script> script = Script::Compile(source);
//    
//    // Run the script to get the result.
//    Handle<Value> result = script->Run();
//    
//    // Convert the result to an UTF8 string and print it.
//    String::Utf8Value utf8(result);
//    float val = (float)result->NumberValue();
    //    float val = (Float)result;
    //    int res = Integer(result);
    
//    console() << val;
    //    printf("%s\n", *utf8);
    
}

Handle<Script> V8Engine::getScript(string sourceStr)
{
    // Get the default Isolate created at startup.
    isolate = Isolate::GetCurrent();
    
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);
    
    //    // Create a new context.
    Handle<Context> context = Context::New(isolate);
    
//    Persistent<Context> persistent_context(isolate, context);
    //
    //    // Enter the context for compiling and running the hello world script.
    Context::Scope context_scope(context);

    // Create a new context.
//    Handle<Context> context = Context::New(isolate);
    
    // Enter the context for compiling and running the hello world script.
//    Context::Scope context_scope(context);
    
    // Create a string containing the JavaScript source code.
    Handle<String> source = String::NewFromOneByte(isolate, (unsigned char *)sourceStr.c_str());
    
    // Compile the source code.
    Handle<Script> script = Script::Compile(source);
    
    return script;
    
    // Run the script to get the result.
//    Handle<Value> result = script->Run();
    
    // Convert the result to an UTF8 string and print it.
//    String::Utf8Value utf8(result);
//    float val = (float)result->NumberValue();
}
