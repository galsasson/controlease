//
//  Program.h
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#ifndef __Controlease__Program__
#define __Controlease__Program__

#include <iostream>
#include <stdio.h>
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include <boost/container/vector.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "OscSender.h"
#include "OscListener.h"

#include "Controlease.h"
#include "ConnectionResult.h"
#include "ResourceManager.h"
#include "CanvasComponent.h"
#include "TextInput.h"
#include "ProgramInput.h"
#include "ProgramOutput.h"
#include "InputNode.h"
#include "OutputNode.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Program : public CanvasComponent
{
public:
    Program(Vec2f _pos);
    ~Program();
    
    void setupConnection(string host, int oport);
    void update();
    void draw();
    void drawOutline();

    void addressInputSet(void);
    void onReturn(void);

	void mouseDown( cease::MouseEvent event );
	void mouseUp( cease::MouseEvent event);
	void mouseWheel( cease::MouseEvent event );
	void mouseMove( cease::MouseEvent event );
	void mouseDrag( cease::MouseEvent event );
    bool isDragPoint(cease::MouseEvent event);
    bool isHotspot(cease::MouseEvent event);
    KeyboardListener* getCurrentKeyboardListener();
    
    float getValue(int i);
    void setValue(int i, float v);
    
    

private:
    void connect();
    void handleMessages();
    void handleAlive(osc::Message msg);
    void addInput(osc::Message msg);
    void addOutput(osc::Message msg);
    void handleOutputMessage(osc::Message msg);
    void resizeComponent();
    
    string programHost;
    int programPort;
    int listenPort;
    
    osc::Sender *oscSender;
    osc::Listener oscListener;

    TextInput *addressInput;
    
    bool connected;
    vector<ProgramInput*> inputs;
    vector<ProgramOutput*> outputs;
    
    bool isEditing;
    Rectf textInputRect;
    
    // graphical representation
    void setSize(Vec2f s);
    
    Rectf localRect;
    Rectf titleRect;
    string name;
    Rectf nameRect;
    Vec2f nextInputPos;
    Vec2f nextOutputPos;
    
    // interaction
    Vec2f prevMouse;
    
    
};

#endif /* defined(__tunnelvars__Program__) */
