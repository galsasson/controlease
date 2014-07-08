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

//#define ALWAYS_UPDATE

class Program : public CanvasComponent
{
public:
    Program(Canvas *c);
    ~Program();
    
    void initNew(Vec2f pos);
    void initFromXml(const XmlTree& xml, bool createNodes = true);
    XmlTree getXml();
        
    void setupConnection(string host, int oport);
    void update();
    void draw();
    void drawOutline();

    void addressInputSet(void);
    void onReturn(void);

    void mouseDown(const cease::MouseEvent& event);
	void mouseDrag(const cease::MouseEvent& event);
    bool isDragPoint(const cease::MouseEvent& event);
    bool isHotspot(const cease::MouseEvent& event);
    KeyboardListener* getCurrentKeyboardListener();
    
    float getValue(int i);
    void setValue(int i, float v);
    
private:
    bool createSenderListener();
    void sendHelloMessage();
    void sendAliveMessage();
    void handleMessages();
    void handleAlive(osc::Message msg);
    void addInput(osc::Message msg);
    void addOutput(osc::Message msg);
    void handleOutputMessage(osc::Message& msg);
    bool doesInputNameExists(std::string name);
    bool doesOutputNameExists(std::string name);
    
    void sendAllInputs();
    
    string programHost;
    int programPort;
    int listenPort;
    
    osc::Sender *oscSender;
    osc::Listener oscListener;

    TextInput *addressInput;
    
    bool bConnected;
    bool bEditing;
    vector<ProgramInput*> inputs;
    vector<ProgramOutput*> outputs;
    
    bool bRunThread;
    std::thread *outputThread;
    void outputThreadWorker();

    // interaction
    Vec2f prevMouse;
    
    bool bSenderRecieverInitialized;
};

#endif /* defined(__tunnelvars__Program__) */
