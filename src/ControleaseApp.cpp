#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "Canvas.h"
#include "Program.h"
#include "Slider.h"
#include "Constant.h"

using namespace ci;
using namespace ci::app;
using namespace std;

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
    
    
private:
    Canvas *canvas;
    
    MouseEvent bakeNewEvent(MouseEvent event, Vec2f origin);
    MouseEvent bakeCanvasMouseEvent(MouseEvent event);
    
};

void ControleaseApp::prepareSettings(cinder::app::AppBasic::Settings *settings)
{
    settings->setWindowSize(800, 600);
    settings->setFrameRate(30);
}

void ControleaseApp::setup()
{
    ResourceManager::getInstance().initResources();
    
    canvas = new Canvas();
    canvas->setup(Vec2f(150, 50), Vec2f(650, 550), Vec2f(750*4, 550*4));
    
    canvas->addProgram(new Program(8617, 9876, Vec2f(450, 250)));
    canvas->addComponent(new Constant(Vec2f(200, 250), Vec2f(130, 40)));
    
    // rendering settings
    glEnable(GL_LINE_SMOOTH);
    gl::enableAlphaBlending();
}

void ControleaseApp::update()
{
    canvas->update();
}

void ControleaseApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0.7, 0.7, 0.7 ) );
    
    canvas->draw();
    
}


void ControleaseApp::mouseDown( MouseEvent event )
{
    canvas->appMouseDown(event);
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
