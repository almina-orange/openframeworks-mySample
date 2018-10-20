/*** Ref: https://drive.google.com/file/d/0BzyVHU69QO3mN3FMTnItN014Z3M/view ***/

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(0, 0, 0);

    fft.setup(pow(2.0, 12.0));

    gui.setup();
    gui.add(saturation.setup("Saturation", 127, 0, 255));
    gui.loadFromFile("setting.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
    fft.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    vector <float> buffer;
    buffer = fft.getBins();

    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofSetLineWidth(ofGetWidth() / (float)(buffer.size() / 2.0 - 0.5));
    ofSetCircleResolution(64);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (int i = 0; i < buffer.size(); i++) {
        float hue = ofMap(i, 0, buffer.size(), 0, 160);
        float br = ofMap(buffer[i], 0, 1, 20, 255);
        // float rad = ofMap(buffer[i], 0, 1, 0, ofGetHeight() / 2.0);
        float rad = ofMap(buffer[i], 0, 1, 0, ofGetHeight() / 3.0);
        float rx = ofMap(i, 0, buffer.size(), 0,  ofGetWidth() / 2.0);
        float lx = ofMap(i, 0, buffer.size(), 0, -ofGetWidth() / 2.0);

        ofSetColor(ofColor::fromHsb(hue, saturation, br));
        ofDrawCircle(rx, 0, rad);
        ofDrawCircle(lx, 0, rad);
    }
    ofPopMatrix();
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
