#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "ofxFaceTracker.h"
#include "ofxFaceTrackerThreaded.h"


class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofVideoGrabber cam;
	ofxFaceTrackerThreaded tracker;
    
    int numPieces = 16;
    ofImage picassoPieces [16];
    int piecePosition [16];
};
