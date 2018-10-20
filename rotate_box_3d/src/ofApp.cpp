/*** Ref: https://qiita.com/Hiroki11x/items/dc0c7250def30f0f48cd ***/

#include "ofApp.h"

#define NUM 100

//--------------------------------------------------------------
void ofApp::setup(){
    // Initial setting
    // ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetBackgroundAuto(false);
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    // Initial variables
    w = ofGetWidth();  h = ofGetHeight();
    // isRecord = false;  cnt = 1;

    // Set boxes
    int boxSize = 3;
    for (int i = 0; i < NUM; i++) {
        float width = ofRandom(-300, 300);
        float height = ofRandom(-300, 300);
        float depth = ofRandom(-300, 300);

        mBox[i].setPosition(width, height, depth);  // 3d position
        mBox[i].set(boxSize);
    }

    // video recorder settings
    // filename = "out.mov";  // ".mov" not supported by twitter
    filename = "out.mp4";
    isRecord = false;

    // please convert video codec in "libx264" for upload --> `ffmpeg -i out.mp4 -vcodec libx264 outConv.mp4`
    vidRecorder.setVideoCodec("mpeg4");
    vidRecorder.setVideoBitrate("800k");
    // vidRecorder.setVideoCodec("libx264");  // video codec for upload
    // vidRecorder.setPixelFormat("yuv420p");  // pixel format for playing ".mp4" on QuickTime
    ofAddListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);

    recFbo.allocate(w, h, GL_RGB);
    recFbo.begin();  ofClear(0, 0, 0, 255);  recFbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){
    // record video, and check
    if(isRecord){
		ofPixels recPix;
        recFbo.readToPixels(recPix);
        bool success = vidRecorder.addFrame(recPix);
        if (!success) { ofLogWarning("This frame was not added!"); }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // cover black filter
    ofSetColor(0, 0, 0, 20);
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    ofDrawRectangle(0, 0, w, h);  // filtering
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    // ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    // ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    // draw boxes
    ofTranslate(w / 2, h / 2);  // shift center position
    ofRotateYDeg(ofGetFrameNum());  // rotate boxes
    ofSetColor(ofColor::fromHsb(ofGetFrameNum() % 225, 225, 225), 175);
    for (int i = 0; i < NUM; i++) { mBox[i].draw(); }

    // // screenshot
    // if (isRecord) {
    //     string filename;
    //     stringstream ss;
    //     ss << "out_" << setfill('0') << setw(3) << right << to_string(cnt++) << ".png";
    //     ss >> filename;
    //     img.grabScreen(0, 0, w, h);
    //     img.save(filename);
    // }
    
    // record buffer
    ofImage recImg;
    recImg.grabScreen(0, 0, w, h);
    recFbo.begin();  ofClear(0, 0, 0, 255);
    recImg.draw(0, 0, w, h);
    recFbo.end();
}

//--------------------------------------------------------------
void ofApp::exit(){
    ofRemoveListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    vidRecorder.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'r'){
        isRecord = !isRecord;
        if (isRecord && !vidRecorder.isInitialized()) {
            // {sampleRate, channels} = 0 (if record only video)
            vidRecorder.setup(filename, w, h, 60, 0, 0);
            vidRecorder.start();
        }
    }
    if (key == 'c'){
        isRecord = false;
        vidRecorder.close();
    }
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
void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    cout << "The recoded video file is now complete." << endl;
}
