#pragma once

#include "ofMain.h"
#include "ofxVideoRecorder.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

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

	    void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);
		
		ofBoxPrimitive mBox[100];
		int w, h;

		int cnt;
		ofImage img;
		bool isRecord;

		ofxVideoRecorder vidRecorder;
		ofFbo recFbo;
		string filename;
		ofImage recImg;
};
