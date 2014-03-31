#pragma once

#include "ofMain.h"
#include "ofxControlease.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofxControlease control;
    
    float bDraw;
    float clearBackground;
    float red, green, blue, alpha;
    float a1, a2;
    float rad1, rad2;
    ofVec2f mousePos;
    
};
