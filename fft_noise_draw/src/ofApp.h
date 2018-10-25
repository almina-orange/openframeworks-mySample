#pragma once

#include "ofMain.h"
#include "ofxProcessFFTCustom.hpp"
#include "ofxGui.h"
#include "ofxSoundObjects.h"
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

		void resolutionChanged(int &resolution);
		void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);
		
		// image
		ofImage lowNoiseImg;
		ofImage midNoiseImg;
		ofImage highNoiseImg;

		// gui parameter
		ofxPanel gui;
		ofxFloatSlider level;
		ofxIntSlider resolution;
		ofxFloatSlider noiseFrequency;
		ofxFloatSlider audioVolume;
		ofxToggle isDrawFFTDebug;
		ofxToggle isUseAudiofile;

		// sound, FFT
		ProcessFFTCustom fft;
		ofSoundPlayer mySound;
		ofSoundBuffer buff;
		ofSoundStream stream;

		// video recorder
		int width, height;
		ofxVideoRecorder vidRecorder;
		bool isRec;
		string filename;
		ofFbo recFbo;
};
