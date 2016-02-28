#include "ofApp.h"

using namespace ofxCv;

void ofApp::setup() {
	cam.initGrabber(1280, 720);
	tracker.setup();
    
    for(int i=0; i < numPieces; i++)
    {
        picassoPieces[i].allocate(1280, 720, OF_IMAGE_COLOR);
        piecePosition[i] = i;
    }
    
}

void ofApp::update() {
	cam.update();
    
	if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
	}
}

void ofApp::draw() {
    ofSetBackgroundColor(ofColor::black);
    
    if(tracker.getFound())
    {
        ofRectangle faceBox(tracker.getPosition().x - tracker.getScale() * 25, tracker.getPosition().y - tracker.getScale() * 25, tracker.getScale() * 50, tracker.getScale() * 50);
        
        ofPixels videoPixels = cam.getPixels();
        ofPixels pixelTransfer;
        
        int x = 0;
        int y = 0;
        
        //extract squares of face from video
        for(int i=0; i < numPieces; i++)
        {
            videoPixels.cropTo(pixelTransfer,
                               faceBox.getX() + (faceBox.getWidth() / sqrt(numPieces) * x),
                               faceBox.getY() + (faceBox.getHeight() / sqrt(numPieces) * y),
                               faceBox.getWidth() / sqrt(numPieces),
                               faceBox.getHeight() / sqrt(numPieces));
            
            picassoPieces[i].setFromPixels(pixelTransfer);
            
            x++;
            if(x >= sqrt(numPieces))
            {
                x = 0;
                y++;
            }
        }
        
        //mix up squares every ~second
//        if(ofGetFrameNum() % (int)(ofGetFrameRate()) == 0)
        if(ofGetFrameNum() % 50 == 0)
        {
            x = 0;
            y = 0;
            int randomPiece = 0;
            vector<int> piecesLeft;
            for(int i=0; i<numPieces; i++)
                piecesLeft.push_back(i);
            
            for(int i=0; i < numPieces; i++)
            {
                randomPiece = (int)ofRandom(piecesLeft.size());
                piecePosition[i] = piecesLeft[randomPiece];
                piecesLeft.erase(piecesLeft.begin() + randomPiece);
                
                x++;
                if(x >= sqrt(numPieces))
                {
                    x = 0;
                    y++;
                }
            }
        }
        
        x = 0;
        y = 0;
        
        //display picasso face
        for(int i=0; i < numPieces; i++)
        {
            picassoPieces[piecePosition[i]].draw(faceBox.getX() + (faceBox.getWidth() / sqrt(numPieces) * x),
                                                    faceBox.getY() + (faceBox.getHeight() / sqrt(numPieces) * y));
            
            ofNoFill();
            ofSetLineWidth(2.0f);
            ofDrawRectangle(faceBox.getX() + (faceBox.getWidth() / sqrt(numPieces) * x),
                            faceBox.getY() + (faceBox.getHeight() / sqrt(numPieces) * y),
                            faceBox.getWidth() / sqrt(numPieces),
                            faceBox.getHeight() / sqrt(numPieces));
            
            x++;
            if(x >= sqrt(numPieces))
            {
                x = 0;
                y++;
            }
        }
    }
    
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
}

void ofApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
	}
}
