#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "rfftw.h"
#include "fftw.h"

class outputStream :
	public AudioSource
{
public:
	outputStream(fftw_real* newData, int lengthOfNewData);
	~outputStream(void);

	void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
	void releaseResources ();
	void getNextAudioBlock (const AudioSourceChannelInfo &bufferToFill);
	void addNewAudioData(fftw_real* newData, int lengthOfNewData);

private:
	float* arrayWithData;
	float* streamForOutput;
	int lengthOfData;
	int streamPosition;
	int currentSamplesPerBlockExpected;
};

