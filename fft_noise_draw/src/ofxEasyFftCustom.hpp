//
//  ofxEasyFftCustom.hpp
//  myWorkspace
//
//  Created by Hiroya Satake on 2018/10/25.
//

#ifndef ofxEasyFftCustom_hpp
#define ofxEasyFftCustom_hpp

#include "ofxFft.h"


class ofxEasyFftCustom : public ofBaseSoundInput{
public:
	ofxEasyFftCustom();
    ~ofxEasyFftCustom();
	void setup(int bufferSize = 512,
						 fftWindowType windowType = OF_FFT_WINDOW_HAMMING,
						 fftImplementation implementation = OF_FFT_BASIC,
						 int audioBufferSize = 256,
						 int audioSampleRate = 44100);
	void setUseNormalization(bool useNormalization);
	void update();
	vector<float>& getAudio();
	vector<float>& getBins();

    void setBuffer(vector<float> buff);  // [modified!] can set sound buffer
    void resetBuffer();  // [modified!] can reset sound buffer, and use audio input
	
	void audioReceived(float* input, int bufferSize, int nChannels);
	
	ofxFft* fft;
	ofSoundStream stream;

    // [modified]
    vector<float> tmp;
    bool isAudioBuffer;
private:
	bool useNormalization;
	ofMutex soundMutex;
	vector<float> audioFront, audioMiddle, audioBack, audioRaw;
	vector<float> bins;
	
	void normalize(vector<float>& data);

};

#endif /* ofxEasyFftCustom_hpp */
