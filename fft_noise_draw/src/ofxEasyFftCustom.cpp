//
//  ofxEasyFftCustom.cpp
//  myWorkspace
//
//  Created by Hiroya Satake on 2018/10/25.
//

#include "ofxEasyFftCustom.hpp"

//--------------------------------------------------------------
ofxEasyFftCustom::ofxEasyFftCustom()
:useNormalization(true) {
}
ofxEasyFftCustom::~ofxEasyFftCustom(){
    stream.close();
}

//--------------------------------------------------------------
void ofxEasyFftCustom::setup(int bufferSize, fftWindowType windowType, fftImplementation implementation, int audioBufferSize, int audioSampleRate) {
	if(bufferSize < audioBufferSize) {
		ofLogWarning("ofxEasyFftCustom") << "bufferSize (" << bufferSize << ") less than audioBufferSize (" << audioBufferSize << "), using " << audioBufferSize;
		bufferSize = audioBufferSize;
	}
	fft = ofxFft::create(bufferSize, windowType, implementation);
	
	bins.resize(fft->getBinSize());
	
	audioFront.resize(bufferSize);
	audioMiddle.resize(bufferSize);
	audioBack.resize(bufferSize);
	audioRaw.resize(bufferSize);
	
    stream.getDeviceList();
    stream.setup(0, 1, audioSampleRate, audioBufferSize, 2);
    stream.setInput(this);

    isAudioBuffer = false;
}

//--------------------------------------------------------------
void ofxEasyFftCustom::setUseNormalization(bool useNormalization) {
	this->useNormalization = useNormalization;
}

//--------------------------------------------------------------
void ofxEasyFftCustom::update() {
	soundMutex.lock();
    // [modified!] can use audiofile (sound buffer) for input
    ofLogVerbose("ofxProcessFFTCustom") << "update() : " << "using audio buffer : " << isAudioBuffer;
    if (isAudioBuffer) {
        audioFront = tmp;
    } else {
        audioFront = audioMiddle;
    }
	soundMutex.unlock();
	
	fft->setSignal(&audioFront[0]);
	float* curFft = fft->getAmplitude();
	copy(curFft, curFft + fft->getBinSize(), bins.begin());
	normalize(bins);
}

//--------------------------------------------------------------
vector<float>& ofxEasyFftCustom::getAudio() {
	return audioFront;
}

//--------------------------------------------------------------
vector<float>& ofxEasyFftCustom::getBins() {
	return bins;
}

//--------------------------------------------------------------
void ofxEasyFftCustom::audioReceived(float* input, int bufferSize, int nChannels) {
	if(audioRaw.size() > bufferSize) {
		copy(audioRaw.begin() + bufferSize, audioRaw.end(), audioRaw.begin()); // shift old
	}
	copy(input, input + bufferSize, audioRaw.end() - bufferSize); // push new
	copy(audioRaw.begin(), audioRaw.end(), audioBack.begin());
	normalize(audioBack);

	soundMutex.lock();
	audioMiddle = audioBack;
	soundMutex.unlock();
}

//--------------------------------------------------------------
void ofxEasyFftCustom::normalize(vector<float>& data) {
	if(useNormalization) {
		float maxValue = 0;
		for(int i = 0; i < data.size(); i++) {
			if(abs(data[i]) > maxValue) {
				maxValue = abs(data[i]);
			}
		}
		for(int i = 0; i < data.size(); i++) {
			data[i] /= maxValue;
		}
	}
}

//--------------------------------------------------------------
void ofxEasyFftCustom::setBuffer(vector<float> buff){
    ofLogVerbose("ofxEasyFftCustom") << "setBuffer() : ";
    tmp = buff;
    isAudioBuffer = true;
}

//--------------------------------------------------------------
void ofxEasyFftCustom::resetBuffer(){
    isAudioBuffer = false;
}