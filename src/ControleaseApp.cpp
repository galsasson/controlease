#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "Canvas.h"
#include "Program.h"

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
    
};

void ControleaseApp::prepareSettings(cinder::app::AppBasic::Settings *settings)
{
    settings->setWindowSize(800, 600);
    settings->setFrameRate(60);
}

void ControleaseApp::setup()
{
    ResourceManager::getInstance().initResources();
    
    canvas = new Canvas();
    canvas->setup(Vec2f(50, 0), Vec2f(750, 600), Vec2f(750*4, 600*4));
    
    canvas->addProgram(new Program(8617, 9876, Vec2f(10, 10)));
}

void ControleaseApp::update()
{
    canvas->update();
}

void ControleaseApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 1, 1, 1 ) );
    
    canvas->draw();
    
}


void ControleaseApp::mouseDown( MouseEvent event )
{
    canvas->mouseDown(event);
}

void ControleaseApp::mouseUp(MouseEvent event)
{
    canvas->mouseUp(event);
}

void ControleaseApp::mouseWheel( MouseEvent event )
{
    canvas->mouseWheel(event);
}

void ControleaseApp::mouseMove( MouseEvent event )
{
    canvas->mouseMove(event);
}

void ControleaseApp::mouseDrag( MouseEvent event )
{
    canvas->mouseDrag(event);
}



CINDER_APP_NATIVE( ControleaseApp, RendererGl )
