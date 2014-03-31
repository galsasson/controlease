#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    control.setup("Crystalfilm", 6002);
//    control.addInput("Y Offset", &yOffset);
    
    fabricImg.loadImage("art/texture-woven-fabric.jpg");
    shader.load("shaders/PhotoshopMathFP");
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(255);
    ofEnableAlphaBlending();
    
    ofSetColor(0, 0, 0, 100);
    ofFill();
    ofEllipse(100, 100, 50, 50);

    shader.begin();
    ofSetColor(255);
    fabricImg.draw(0, 0, ofGetWidth(), ofGetHeight());
    shader.end();
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