#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetBackgroundAuto(false);
    ofSetVerticalSync(false);
    
    control.setup("Openframeworks Drawing App", 6001);
    
    red = 100;
    control.addInput("Red", &red);
    green = 0;
    control.addInput("Green", &green);
    blue = 0;
    control.addInput("Blue", &blue);
    alpha = 255;
    control.addInput("Alpha", &alpha);
    
    mousePos = ofVec2f(0, 0);
    control.addInput("Mouse X", &mousePos.x);
    control.addInput("Mouse Y", &mousePos.y);
    
    a1 = 0;
    control.addInput("Angle 1", &a1);
    rad1 = 5;
    control.addInput("Radius 1", &rad1);
    a2 = 0;
    control.addInput("Angle 2", &a2);
    rad2 = 5;
    control.addInput("Radius 2", &rad2);
    
    clearBackground = 1;
    control.addInput("Clear Background", &clearBackground);
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
    if (clearBackground > 0) {
        ofBackground(0, 0, 0);
    }
    
    ofPushMatrix();
    ofTranslate(mousePos);
    
    ofSetColor(red, green, blue, alpha);
    ofFill();
    ofEllipse(0, 0, 10, 10);
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}