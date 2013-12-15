#include "outputStream.h"


outputStream::outputStream(fftw_real* newData, int lengthOfNewData)
{
	streamPosition = 0;
	lengthOfData = lengthOfNewData;
	arrayWithData = new float[lengthOfData];
	streamForOutput = new float[lengthOfData];
	for(int i = 0; i < lengthOfData; i++)
		arrayWithData[i] = (float)newData[i];
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
		

	for(int i = 0; i < bufferToFill.numSamples; i++){
		streamForOutput[i] = arrayWithData[(streamPosition + i) % lengthOfData];
		}

		streamPosition += bufferToFill.numSamples;
		streamPosition = streamPosition % lengthOfData;

		
		bufferToFill.buffer->addFrom(0, bufferToFill.startSample, streamForOutput, bufferToFill.numSamples, 1);
	

}

void outputStream::addNewAudioData(fftw_real* newData, int lengthOfNewData){

	
	for(int i = 0; i < lengthOfData; i++)
		arrayWithData[i] = (float)newData[i];
	//lengthOfData = lengthOfNewData;

}

