#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "v8.h"

#include "MenuBar.h"
#include "ComponentBox.h"
#include "Canvas.h"
#include "ComponentFactory.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace v8;

class ControleaseApp : public AppNative {
  public:
    void prepareSettings(Settings *settings);
    
	void setup();
	void update();
    void threadUpdate();
	void draw();
    
	void mouseDown( MouseEvent event );
	void mouseUp( MouseEvent event);
	void mouseWheel( MouseEvent event );
	void mouseMove( MouseEvent event );
	void mouseDrag( MouseEvent event );
	void keyDown( KeyEvent event );
	void keyUp( KeyEvent event );

	void resize();

    void menuButtonClicked(Button* button);

private:

    MenuBar *menuBar;
    Canvas *canvas;
    ComponentBox *compbox;

    Vec2f getMenubarSize();
    Vec2f getCompboxSize();
    Vec2f getCanvasSize();
    
    std::thread updateThread;
};

void ControleaseApp::prepareSettings(cinder::app::AppBasic::Settings *settings)
{
    settings->enableHighDensityDisplay();
    settings->setWindowSize(800, 600);
    settings->setFrameRate(30);
    
    settings->enablePowerManagement(true);
}

void ControleaseApp::setup()
{
    ResourceManager::getInstance().initResources();
    
    menuBar = new MenuBar(Vec2f(0, 0), getMenubarSize());
    menuBar->onButtonClicked(boost::bind(&ControleaseApp::menuButtonClicked, this, _1));
    
    canvas = new Canvas();
    canvas->setup(Vec2f(getCompboxSize().x, getMenubarSize().y), getCanvasSize());
    compbox = new ComponentBox(Vec2f(0, getMenubarSize().y), getCompboxSize());
    
    // rendering settings
    glEnable(GL_LINE_SMOOTH);
    gl::enableAlphaBlending();
    
//    updateThread = std::thread(&ControleaseApp::threadUpdate, this);

}

void ControleaseApp::update()
{
    compbox->update();
    canvas->update();
}

void ControleaseApp::threadUpdate()
{
    while (1)
    {
        compbox->update();
        canvas->update();
        usleep(10000);
//        console() << "update..."<<endl;
    }
}

void ControleaseApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0.7, 0.7, 0.7 ) );
    
    menuBar->draw();
    compbox->draw();
    canvas->draw();
    
}


void ControleaseApp::mouseDown( MouseEvent event )
{
    if (canvas->contains(event.getPos())) {
        canvas->appMouseDown(event);
    }
    else if (compbox->contains(event.getPos())) {
        ComponentButton *button = compbox->mouseDown(event);
        if (button != NULL) {
            canvas->addComponent(ComponentFactory::newComponent(button, canvas, canvas->topLeft));
        }
    }
    else if (menuBar->contains(event.getPos())) {
        menuBar->mouseDown(event);
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
    menuBar->setSize(getMenubarSize());
    compbox->setSize(getCompboxSize());
    canvas->setSize(getCanvasSize());
}

void ControleaseApp::menuButtonClicked(Button* button)
{
    console() << button->text << " was clicked"<<endl;
}

Vec2f ControleaseApp::getMenubarSize()
{
    return Vec2f(getWindowWidth(), 40);
}

Vec2f ControleaseApp::getCompboxSize()
{
    return Vec2f(150, getWindowHeight()-getMenubarSize().y);
}

Vec2f ControleaseApp::getCanvasSize()
{
    return Vec2f(getWindowWidth()-getCompboxSize().x, getWindowHeight()-getMenubarSize().y);
}

CINDER_APP_NATIVE( ControleaseApp, RendererGl )
