/*** Ref: https://drive.google.com/file/d/0BzyVHU69QO3mN3FMTnItN014Z3M/view ***/

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // App settings
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(0, 0, 0);
    ofSetLogLevel(OF_LOG_VERBOSE);

    // GUI setting
    resolution.addListener(this, &ofApp::resolutionChanged);  // function hook
    // isUseAudiofile.addListener(this, &ofApp::inputFlagChanged);
    gui.setup();
    gui.add(level.setup("Input level", 100, 0, 2000));
    gui.add(resolution.setup("Resolution", 256, 2, 1024));
    gui.add(noiseFrequency.setup("Noise frequency", 80.0, 0.0001, 1024.0));
    gui.add(audioVolume.setup("Audio volume", 0.1, 0, 1.0));
    gui.add(isDrawFFTDebug.setup("Draw FFT debug", true));
    // gui.add(isUseAudiofile.setup("Use audio file", true));
    gui.loadFromFile("settings.xml");

    // Initialize image
    lowNoiseImg.allocate(resolution, resolution, OF_IMAGE_GRAYSCALE);
    midNoiseImg.allocate(resolution, resolution, OF_IMAGE_GRAYSCALE);
    highNoiseImg.allocate(resolution, resolution, OF_IMAGE_GRAYSCALE);

    // Get audio buffer from audio file
    /*** using addon "ofSoundfile.h" ***/
    // auto filename = ofToDataPath("ambientSound.wav");
    // mySound.load(filename.data());

    // uint64_t samples = mySound.length();
    // uint64_t sampleRate = mySound.samplerate();
    // int channels = mySound.channels();
    // buff.resize(samples * channels);
	// buff.copyFrom(mySound.data(), samples, channels, sampleRate);

    // ofLogVerbose("Load data check") << mySound.loaded();
    // ofLogVerbose("Number of samples") << mySound.length();
    // ofLogVerbose("Number of channels") << mySound.channels();
    // ofLogVerbose("Sample rate") << mySound.samplerate();
    // ofLogVerbose("Loaded buffer") << buff.size();

    /*** using addon ofxSoundObjects ***/
    // FFT setting (for audio file)
    // auto filename = ofToDataPath("ambientSound.wav");
    // ofxLoadSound(buff, filename.data());
    // ofLogVerbose("ofSoundBuffer") << "Loaded buffer : " << buff.size();

    // fft = ofxFft::create();
    // fft->setSignal(buff.getBuffer());
    // fft.setup(buff.size());
    // fft.setBuffer(buff.getBuffer());

    // FFT setting (for audio input)
    fft.setup();
    fft.setNumFFTBins(16);
    fft.setNormalize(true);

    // Load audio file
    // auto filename = ofToDataPath("ambientSound.wav");
    // mySound.load(filename.data());
    // mySound.setVolume(audioVolume);
    // mySound.setLoop(true);
    // mySound.play();

    // Video recorder settings
    filename = "out.mp4";
    isRec = false;

    // please convert video codec in "libx264" for upload --> `ffmpeg -i out.mp4 -vcodec libx264 outConv.mp4`
    vidRecorder.setVideoCodec("mpeg4");
    vidRecorder.setVideoBitrate("800k");
    ofAddListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);

    // allocate buffer for recording
    width = ofGetWidth();  height = ofGetHeight();
    recFbo.allocate(width, height, GL_RGB);
    recFbo.begin();  ofClear(0, 0, 0, 255);  recFbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){
    // mySound.setVolume(audioVolume);

    fft.update();

    // Make noisy images according to audio values
    float lowValue = ofMap(fft.getLowVal(), 0, 1, 0, level);
    float midValue = ofMap(fft.getMidVal(), 0, 1, 0, level);
    float highValue = ofMap(fft.getHighVal(), 0, 1, 0, level);

    int tmpIndex;
    ofPixels& lowPixels = lowNoiseImg.getPixels();  // [caution] using pointer
    tmpIndex = 0;
    for (int y = 0; y < lowNoiseImg.getHeight(); y++) {
        for (int x = 0; x < lowNoiseImg.getWidth(); x++) {
            float tmpNoise = ofNoise(x / noiseFrequency, y / noiseFrequency, ofGetElapsedTimef());
            lowPixels[tmpIndex] = tmpNoise * lowValue;
            tmpIndex++;
        }
    }
    lowNoiseImg.update();

    ofPixels& midPixels = midNoiseImg.getPixels();  // [caution] using pointer
    tmpIndex = 0;
    for (int y = 0; y < midNoiseImg.getHeight(); y++) {
        for (int x = 0; x < midNoiseImg.getWidth(); x++) {
            float tmpNoise = ofNoise(x / noiseFrequency, y / noiseFrequency, ofGetElapsedTimef() * 2.0);
            midPixels[tmpIndex] = tmpNoise * midValue;
            tmpIndex++;
        }
    }
    midNoiseImg.update();

    ofPixels& highPixels = highNoiseImg.getPixels();  // [caution] using pointer
    tmpIndex = 0;
    for (int y = 0; y < highNoiseImg.getHeight(); y++) {
        for (int x = 0; x < highNoiseImg.getWidth(); x++) {
            float tmpNoise = ofNoise(x / noiseFrequency, y / noiseFrequency, ofGetElapsedTimef() * 4.0);
            highPixels[tmpIndex] = tmpNoise * highValue;
            tmpIndex++;
        }
    }
    highNoiseImg.update();

    // record video, and check
    if(isRec){
		ofPixels recPix;
        recFbo.readToPixels(recPix);
        bool success = vidRecorder.addFrame(recPix);
        if (!success) { ofLogWarning("This frame was not added!"); }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Record buffer
    if (isRec) {
        ofImage recImg;
        recImg.grabScreen(0, 0, width, height);
        recFbo.begin();  ofClear(0, 0, 0, 255);

        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofSetColor(255, 0, 0);  lowNoiseImg.draw(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(0, 255, 0);  midNoiseImg.draw(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(0, 0, 255);  highNoiseImg.draw(0, 0, ofGetWidth(), ofGetHeight());

        recFbo.end();
    }

    // Draw noisy images
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255, 0, 0);  lowNoiseImg.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(0, 255, 0);  midNoiseImg.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(0, 0, 255);  highNoiseImg.draw(0, 0, ofGetWidth(), ofGetHeight());

    // Draw fft status
    if (isDrawFFTDebug) { fft.drawBars();  fft.drawDebug(); }

    // Draw gui
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
    ofRemoveListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    vidRecorder.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'd' ) { isDrawFFTDebug = !(isDrawFFTDebug); }

    if (key == 'r'){
        isRec = !isRec;
        if (isRec && !vidRecorder.isInitialized()) {
            // {sampleRate, channels} = 0 (if record only video)
            // vidRecorder.setup(filename, width, height, 60, 0, 0);
            vidRecorder.setup(filename, width, height, 20, 0, 0);
            vidRecorder.start();
        }
    }
    if (key == 'c'){
        isRec = false;
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
void ofApp::windowResized(int width, int height){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::resolutionChanged(int &resolution){ 
    // re-allocate variables in resolution
    lowNoiseImg.allocate(resolution, resolution, OF_IMAGE_GRAYSCALE);
    midNoiseImg.allocate(resolution, resolution, OF_IMAGE_GRAYSCALE);
    highNoiseImg.allocate(resolution, resolution, OF_IMAGE_GRAYSCALE);
}

// //--------------------------------------------------------------
// void ofApp::inputFlagChanged(bool &flag){ 
//     if (flag) {
//         /*** using addon ofxSoundObjects ***/
//         // auto filename = ofToDataPath("ambientSound.wav");
//         // ofxLoadSound(buff, filename.data());
//         // ofLogVerbose("ofSoundBuffer") << "Loaded buffer : " << buff.size();

//         // fft.setup(buff.size());
//         // fft.setBuffer(buff.getBuffer());
//     }
//     ofLogVerbose("Flag") << flag;
// }

//--------------------------------------------------------------
void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    cout << "The recoded video file is now complete." << endl;
}
