#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class outputStream :
	public AudioSource
{
public:
	outputStream(void);
	~outputStream(void);

	void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
	void releaseResources ();
	void getNextAudioBlock (const AudioSourceChannelInfo &bufferToFill);
	void addNewAudioData(float* newData, int lengthOfNewData);

private:
	bool dataReady;
	float* arrayWithData;
	float* streamForOutput;
	int lengthOfData;
	int streamPosition;
	int currentSamplesPerBlockExpected;
};

