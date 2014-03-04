#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "v8.h"

#include "Canvas.h"
#include "ToolBox.h"
#include "Program.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace v8;

class ControleaseApp : public AppNative {
  public:
    void prepareSettings(Settings *settings);
    
	void setup();
	void update();
	void draw();
    
	void mouseDown( MouseEvent event );
	void mouseUp( MouseEvent event);
	void mouseWheel( MouseEvent event );
	void mouseMove( MouseEvent event );
	void mouseDrag( MouseEvent event );
	void keyDown( KeyEvent event );
	void keyUp( KeyEvent event );

	void resize();
    
private:
    Vec2f getCanvasSize();
    Vec2f getToolboxSize();
    
    Canvas *canvas;
    ToolBox *toolbox;
    
    MouseEvent bakeNewEvent(MouseEvent event, Vec2f origin);
    MouseEvent bakeCanvasMouseEvent(MouseEvent event);
    
};

void ControleaseApp::prepareSettings(cinder::app::AppBasic::Settings *settings)
{
    settings->enableHighDensityDisplay();
    settings->setWindowSize(800, 600);
    settings->setFrameRate(30);
}

void ControleaseApp::setup()
{
    ResourceManager::getInstance().initResources();
    
    canvas = new Canvas();
    canvas->setup(Vec2f(150, 50), getCanvasSize());
    toolbox = new ToolBox(Vec2f(0, 50), getToolboxSize());
    
    // rendering settings
    glEnable(GL_LINE_SMOOTH);
    gl::enableAlphaBlending();
}

void ControleaseApp::update()
{
    toolbox->update();
    canvas->update();
}

void ControleaseApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0.7, 0.7, 0.7 ) );
    
    toolbox->draw();
    canvas->draw();
    
}


void ControleaseApp::mouseDown( MouseEvent event )
{
    if (canvas->contains(event.getPos())) {
        canvas->appMouseDown(event);
    }
    else if (toolbox->contains(event.getPos())) {
        Tool *tool = toolbox->mouseDown(event);
        if (tool != NULL) {
            canvas->addComponent(tool);
        }
    }
}

void ControleaseApp::mouseUp(MouseEvent event)
{
    canvas->appMouseUp(event);
}

void ControleaseApp::mouseWheel( MouseEvent event )
{
    canvas->appMouseWheel(event);
}

void ControleaseApp::mouseMove( MouseEvent event )
{
    canvas->appMouseMove(event);
}

void ControleaseApp::mouseDrag( MouseEvent event )
{
    canvas->appMouseDrag(event);
}

void ControleaseApp::keyDown( KeyEvent event )
{
    canvas->appKeyDown(event);
}

void ControleaseApp::keyUp( KeyEvent event )
{
    canvas->appKeyUp(event);
}

void ControleaseApp::resize()
{
    toolbox->setSize(getToolboxSize());
    canvas->setSize(getCanvasSize());
}

Vec2f ControleaseApp::getCanvasSize()
{
    return Vec2f(getWindowWidth()-150, getWindowHeight()-50);
}

Vec2f ControleaseApp::getToolboxSize()
{
    return Vec2f(150, getWindowHeight()-50);
}

MouseEvent ControleaseApp::bakeNewEvent(MouseEvent event, Vec2f origin)
{
    Vec2i p = event.getPos() - origin;
    return MouseEvent(event.getWindow(), 0, p.x, p.y, event.getNativeModifiers(), event.getWheelIncrement(), event.getNativeModifiers() );
}

MouseEvent ControleaseApp::bakeCanvasMouseEvent(cinder::app::MouseEvent event)
{
    Vec2f p = canvas->topLeft + (Vec2f)event.getPos() / canvas->scale - canvas->pos;
    return MouseEvent(event.getWindow(), 0, p.x, p.y, event.getNativeModifiers(), event.getWheelIncrement(), event.getNativeModifiers() );
}




CINDER_APP_NATIVE( ControleaseApp, RendererGl )
