#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSyphon.h"
#include "ofxAbletonLive.h"

#include "frameStructure.h"

#define F_NUMBER 24

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
     
     FrameStr frameStr[F_NUMBER];
     
     ofEasyCam cam;
     
     ofBoxPrimitive scanBox;
     ofRectangle scanRect;
     
     ofFbo outputFbo;
     ofPixels pix;
     ofPixels rePix;
     
     int radius;
     int scanType;
     float scanPos;
     void scan(int _scanType);
     ofFloatColor setColor;
     ofFloatColor darkColor;
     int moveRenge;
     int startPos;
     
     //video
     ofVideoPlayer video;
     ofPixels vidPix;
     ofFbo vidFbo;
     ofFbo vidFbo_Side;
     ofFbo vidFbo_Top;
     int fizPrmX;
     int mapType;
     
     //Gui
     ofxPanel gui;
     ofParameter<bool> isStart;
     ofParameter<bool> isVisibleScaning;
     ofParameter<bool> isVisibleGrid;
     ofxButton modeScan;
     ofxButton modeVideoMap;
     ofParameter<float> speed;
     ofParameter<ofFloatColor> color;
     ofParameter<bool> flipCol;
     ofxButton scanXaxis;
     ofxButton scanYaxis;
     ofxButton scanZaxis;
     ofxButton sideviewMap;
     ofxButton topviewMap;
     ofParameter<bool> AbletonOn;
     ofParameter<string> clipNumber;
     
     int mode = 0;
     
     //Syphon
     ofxSyphonServer server;
     
     ofxAbletonLive live;
     int stopper;
		
};
