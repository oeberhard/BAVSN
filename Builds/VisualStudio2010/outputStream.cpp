#include "outputStream.h"


outputStream::outputStream(void)
{
}


outputStream::~outputStream(void)
{
}

void outputStream::prepareToPlay (int samplesPerBlockExpected, double sampleRate){

	currentSamplesPerBlockExpected = samplesPerBlockExpected;

}

void outputStream::releaseResources (){
}

void outputStream::getNextAudioBlock (const AudioSourceChannelInfo &bufferToFill){
		
	if(dataReady){

	for(int i = streamPosition; i < bufferToFill.numSamples; i++){
		streamForOutput[i] = arrayWithData[streamPosition % lengthOfData];
		streamPosition++;
	}
		bufferToFill.buffer->addFrom(0, bufferToFill.startSample, streamForOutput, bufferToFill.numSamples, 1);
	}

}

void outputStream::addNewAudioData(float* newData, int lengthOfNewData){

	arrayWithData = newData;
	lengthOfData = lengthOfNewData;
	dataReady = true;

}

