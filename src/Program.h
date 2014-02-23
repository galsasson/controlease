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
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include <boost/container/vector.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "OscSender.h"
#include "OscListener.h"

#include "MouseListener.h"
#include "ProgramInput.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Program : public MouseListener
{
public:
    Program(int oport, int iport, Vec2f _pos);
    ~Program();
    
    void update();
    void draw();
    
    bool contains(Vec2f p);
    
	void mouseDown( cease::MouseEvent event );
	void mouseUp( cease::MouseEvent event);
	void mouseWheel( cease::MouseEvent event );
	void mouseMove( cease::MouseEvent event );
	void mouseDrag( cease::MouseEvent event );

private:
    void connect();
    void handleMessages();
    void handleAlive(osc::Message msg);
    void addInput(osc::Message msg);
    
    int programPort;
    int listenPort;
    
    osc::Sender *oscSender;
    osc::Listener oscListener;

    bool connected;
    boost::container::vector<ProgramInput*> inputs;
//    boost::container::vector<ProgramOutput> outputs;
    
    // graphical representation
    void setSize(Vec2f s);
    
    string name;
    Vec2f nameSize;
    Vec2f halfNameSize;
    Vec2f pos;
    Vec2f size;
    Vec2f halfSize;
    Vec2f nextInputPos;
    
    // interaction
    Vec2f prevMouse;
    bool isMouseDown;
    
    
};

#endif /* defined(__tunnelvars__Program__) */
