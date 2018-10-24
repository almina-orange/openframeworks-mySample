/*** Ref: https://drive.google.com/file/d/0BzyVHU69QO3mN3FMTnItN014Z3M/view ***/

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(0, 0, 0);
    ofSetLogLevel(OF_LOG_VERBOSE);

    // FFT setting
    fft.setup();
    fft.setNumFFTBins(16);
    fft.setNormalize(true);

    // GUI setting
    resolution.addListener(this, &ofApp::resolutionChanged);
    gui.setup();
    gui.add(level.setup("Input level", 100, 0, 2000));
    gui.add(resolution.setup("Resolution", 256, 2, 1024));
    gui.add(noiseFrequency.setup("Noise frequency", 80.0, 0.0001, 1024.0));
    gui.add(isDebug.setup("Draw debug status", true));
    gui.loadFromFile("settings.xml");

    // Initialize image
    lowNoiseImg.allocate(resolution, resolution, OF_IMAGE_GRAYSCALE);
    midNoiseImg.allocate(resolution, resolution, OF_IMAGE_GRAYSCALE);
    highNoiseImg.allocate(resolution, resolution, OF_IMAGE_GRAYSCALE);
}

//--------------------------------------------------------------
void ofApp::update(){
    fft.update();

    float lowValue = ofMap(fft.getLowVal(), 0, 1, 0, level);
    float midValue = ofMap(fft.getMidVal(), 0, 1, 0, level);
    float highValue = ofMap(fft.getHighVal(), 0, 1, 0, level);

    // ofLogVerbose("ofApp") << "low value : " << lowValue;
    // ofLogVerbose("ofApp") << "middle value : " << midValue;
    // ofLogVerbose("ofApp") << "high value : " << highValue << "\n";

    int tmpIndex;
    ofPixels& lowPixels = lowNoiseImg.getPixels();
    tmpIndex = 0;
    for (int y = 0; y < lowNoiseImg.getHeight(); y++) {
        for (int x = 0; x < lowNoiseImg.getWidth(); x++) {
            float tmpNoise = ofNoise(x / noiseFrequency, y / noiseFrequency, ofGetElapsedTimef());
            lowPixels[tmpIndex] = tmpNoise * lowValue;
            tmpIndex++;
        }
    }
    lowNoiseImg.update();

    ofPixels& midPixels = midNoiseImg.getPixels();
    tmpIndex = 0;
    for (int y = 0; y < midNoiseImg.getHeight(); y++) {
        for (int x = 0; x < midNoiseImg.getWidth(); x++) {
            float tmpNoise = ofNoise(x / noiseFrequency, y / noiseFrequency, ofGetElapsedTimef() * 2.0);
            midPixels[tmpIndex] = tmpNoise * midValue;
            tmpIndex++;
        }
    }
    midNoiseImg.update();

    ofPixels& highPixels = highNoiseImg.getPixels();
    tmpIndex = 0;
    for (int y = 0; y < highNoiseImg.getHeight(); y++) {
        for (int x = 0; x < highNoiseImg.getWidth(); x++) {
            float tmpNoise = ofNoise(x / noiseFrequency, y / noiseFrequency, ofGetElapsedTimef() * 4.0);
            highPixels[tmpIndex] = tmpNoise * highValue;
            tmpIndex++;
        }
    }
    highNoiseImg.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255, 0, 0);  lowNoiseImg.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(0, 255, 0);  midNoiseImg.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(0, 0, 255);  highNoiseImg.draw(0, 0, ofGetWidth(), ofGetHeight());

    if (isDebug) {
        fft.drawDebug();
        fft.drawBars();
    }

    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'd') {  isDebug = !isDebug;  std::cout << "ofApp : [Pressed key]" << endl;  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::resolutionChanged(int &resolution){ 
    lowNoiseImg.allocate(resolution, resolution, OF_IMAGE_GRAYSCALE);
    midNoiseImg.allocate(resolution, resolution, OF_IMAGE_GRAYSCALE);
    highNoiseImg.allocate(resolution, resolution, OF_IMAGE_GRAYSCALE);
}
