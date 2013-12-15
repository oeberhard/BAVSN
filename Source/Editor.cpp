/* 
 * File:   Editor.cpp
 * Author: Boom
 * 
 * Created on 22. Oktober 2012, 21:28
 */

#include "Editor.h"

Editor::Editor(int new_numberOfFrequencyBins, LoadNAnalyseFileThread* thread) {
//    soundFileInfo = 0;
    spectralArray = 0;
    spectogram = 0;
    numberOfSpectralSamples = 0;
    lenghtOfFourierTransform = new_numberOfFrequencyBins;
    notesChanged = true; 
	containingThread = thread;
	numberOfKFrequencies = 0;
	originalStream = 0;
	resampledStream = 0;
	imageSpectogramFromNotes = 0;
	imageImpulseDistFromNotes = 0;
	arrayOfComponents = 0;
	backgroundSoundIsActive = true;
	inputSamplerate = 0;
	isLoadingData = false;
	backgroundGain = 1;
	srand(time(NULL));
	


}

Editor::Editor(const Editor& orig) {
}

Editor::~Editor() {
}

void Editor::separateNotesFromFile(File waveFile, int numberOfComponents, int numberOfKernelFrequencies, int numberOfKs, int numberOfIterations, int numberOfFixedPointIterations) {




    loadWaveFile(waveFile);
    arrayOfComponents = separateNotes(numberOfComponents, numberOfKernelFrequencies, numberOfKs, numberOfIterations, numberOfFixedPointIterations);

	numberOfKFrequencies = numberOfKernelFrequencies;
}


void Editor::loadWaveFile(File new_waveFile) {



	
		
	//-----------juce loader
		WavAudioFormat* _wavAudioFormat = new WavAudioFormat(); 
		_audioFormatReader = _wavAudioFormat->createReaderFor( new_waveFile.createInputStream(), false ); 
		int** _wavData = new int* [ 3 ]; 
		_wavData[ 0 ] = new int[ _audioFormatReader->lengthInSamples ]; 
		_wavData[ 1 ] = new int[ _audioFormatReader->lengthInSamples ]; 
		_wavData[ 2 ] = 0; 
		_audioFormatReader->read( _wavData, 1, 0, _audioFormatReader->lengthInSamples, true ); 

		inputSamplerate = _audioFormatReader->sampleRate;

		soundFramesArray;
		//soundFramesArray = new fftw_real[_audioFormatReader->lengthInSamples];

		//alt
		soundFramesArray = (double*) malloc(sizeof (double) *(int) _audioFormatReader->lengthInSamples);
		//---

		//Output array
		lengthOfInputArray = _audioFormatReader->lengthInSamples;
		timeDomainSignalForOutput = new fftw_real[_audioFormatReader->lengthInSamples];

		//transform to float

		float ** waveAsFloat = (float**)_wavData;

		for(int i=0; i<_audioFormatReader->lengthInSamples; i++)
				soundFramesArray[i] = waveAsFloat[ 0 ][ i ];

	/*	if(_audioFormatReader->usesFloatingPointData){

			fftw_real ONEOVERINTMAX = pow((double)2,(int) _audioFormatReader->bitsPerSample)/2;

			for(int i=0; i<_audioFormatReader->lengthInSamples; i++){
				soundFramesArray[i] = _wavData[ 0 ][ i ] / ONEOVERINTMAX;
			}
		}*/
		//else
		//{
		//	for(int i=0; i<_audioFormatReader->lengthInSamples; i++){
		//		soundFramesArray[i] = _wavData[ 0 ][ i ];
		//	}
		//}


// alternative libsndfile--------------
   //*SNDFILE *waveFile;
   // double* soundFramesArray;
   // soundFileInfo = (SF_INFO *) malloc(sizeof (SF_INFO));
   // soundFileInfo->format = 0;

   // if (!(waveFile = sf_open(fileName, SFM_READ, soundFileInfo))) {
   //     printf("error opening input file/n");
   //     exit(-1);
   // }
   // soundFramesArray = (double*) malloc(sizeof (double) *(int) soundFileInfo->frames); //new float[soundFileInfo->frames];
   // 
   // int framesCount = sf_readf_double(waveFile, soundFramesArray, soundFileInfo->frames);
//-------------------------------------
    sizeOfSpectralArray = 1024 * (ceilf(_audioFormatReader->lengthInSamples / 256)+1 );
    numberOfSpectralSamples = (ceilf(_audioFormatReader->lengthInSamples / 256)+1 );

    spectralArray = new fftw_real[sizeOfSpectralArray];
	originalSpectralArray = new fftw_real[sizeOfSpectralArray];

    oneDimensionalSpectralArrayForOutput = new fftw_real[sizeOfSpectralArray];

	transformToFD(soundFramesArray, spectralArray, _audioFormatReader->lengthInSamples, 256);

	//copy spectralArray
	for(int i = 0; i < sizeOfSpectralArray; i++){
		originalSpectralArray[i] = spectralArray[i];
	}
	//-------

   // sf_close(waveFile);

    transformToSpectogram(spectralArray);

    printSpectogramToFile();

}



void Editor::transformToFD(fftw_real *inputArray, fftw_real* outputArray, int sizeOfInput, int hopsize) {

    //create hanning window //changed 1. märz
    window = new fftw_real[lenghtOfFourierTransform];
    for (int i = 0; i < lenghtOfFourierTransform; i++)
        window[i] = 0.5 - 0.5 * cos(6.283 * ((float) i / (lenghtOfFourierTransform - 1)));
    
	Stft stft;

    int posout = stft.doStft(inputArray, window, outputArray, sizeOfInput, lenghtOfFourierTransform, hopsize);
    
	
	// free(window);

}

void Editor::transformToSpectogram(fftw_real* spectralArrayFromFttw) {

    spectogram = new float*[lenghtOfFourierTransform / 2];
    for (int i = 0; i < lenghtOfFourierTransform / 2; ++i)
        spectogram[i] = new float[numberOfSpectralSamples];

    for (int i = 0; i < lenghtOfFourierTransform / 2; i++) {
        for (int k = 0; k < numberOfSpectralSamples; k++) {
            spectogram[i][k] = 0;
        }
    }

	//added sqrt 
    for (int i = 0; i < numberOfSpectralSamples; i++) {
        for (int k = 1; k < lenghtOfFourierTransform / 2; k++) {
            spectogram[k][i] = sqrt(spectralArrayFromFttw[i * lenghtOfFourierTransform + k * 2] * spectralArrayFromFttw[i * lenghtOfFourierTransform + k * 2] +
                    spectralArrayFromFttw[i * lenghtOfFourierTransform + k * 2 + 1] * spectralArrayFromFttw[i * lenghtOfFourierTransform + k * 2 + 1]);
        }
    }
}

MskComponent* Editor::separateNotes(int numberOfComponents, int numberOfKernelFrequencies, int numberOfKs, int numberOfIterations, int numberOfFixedPointIterations) {
    nS = new NoteSeparator(numberOfComponents, numberOfKernelFrequencies, numberOfKs, lenghtOfFourierTransform / 2, numberOfSpectralSamples, spectogram, containingThread);

    // Allocates Fields because dimensions where not known when editor was instatiated

    impulseDistributionFromNotes = new float*[nS->numberOfKs];
    for (int i = 0; i < nS->numberOfKs; ++i)
        impulseDistributionFromNotes[i] = new float[nS->numberOfSamples];

    spectogramFromNotes = new float*[nS->numberOfFrequencies];
    for (int i = 0; i < nS->numberOfFrequencies; ++i)
        spectogramFromNotes[i] = new float[nS->numberOfSamples];



    MskComponent* processedComponents = nS->updateCycle(numberOfIterations, numberOfFixedPointIterations);
    return processedComponents;
}

void Editor::printSpectogramToFile() {

    //find highest
    float highestValue = 0;
    for (int i = 0; i < lenghtOfFourierTransform / 2; i++) {
        for (int k = 0; k < numberOfSpectralSamples; k++) {
            if (spectogram[i][k] > highestValue)
                highestValue = spectogram[i][k];
        }
    }

    float multiplicationFactor = 250 / highestValue;
    BMP newBMP;
    newBMP.SetSize(lenghtOfFourierTransform / 2, numberOfSpectralSamples);
    newBMP.SetBitDepth(8);
    for (int i = 0; i < lenghtOfFourierTransform / 2; i++) {
        for (int k = 0; k < numberOfSpectralSamples; k++) {

            if (spectogram[i][k] * multiplicationFactor < 256) {
                newBMP(i, k)->Blue = int(spectogram[i][k] * multiplicationFactor);
                newBMP(i, k)->Alpha = int(spectogram[i][k] * multiplicationFactor);
                newBMP(i, k)->Green = int(spectogram[i][k] * multiplicationFactor);
                newBMP(i, k)->Red = int(spectogram[i][k] * multiplicationFactor);
            } else {
                newBMP(i, k)->Blue = 250;
                newBMP(i, k)->Alpha = 250;
                newBMP(i, k)->Green = 250;
                newBMP(i, k)->Red = 250;
            }

            //cout << impulsDistribution[i][k] << endl;
        }
    }
    newBMP.WriteToFile("Spectogram.bmp");

}
///////////////

/*
BMP newBMP;
newBMP.SetSize(lenghtOfFourierTransform/2, numberOfSpectralSamples);
newBMP.SetBitDepth(8);
for (int i = 0; i < lenghtOfFourierTransform/2; i++) {
    for (int k = 0; k < numberOfSpectralSamples; k++) {
        if (spectogram[i][k] > 0.00000001) {
            newBMP(i, k)->Blue = 100;
            newBMP(i, k)->Alpha = 1;
            newBMP(i, k)->Green = 1;
            newBMP(i, k)->Red = 1;
        } 
        else {
            newBMP(i, k)->Blue = 1;
            newBMP(i, k)->Alpha = 1;
            newBMP(i, k)->Green = 1;
            newBMP(i, k)->Red = 1;

        }
    }
}
newBMP.WriteToFile("C:\\Users\\Boom\\Desktop\\Spectogram.bmp");
}*/

////////////////////////////

void Editor::findNotesWithOtherParameters(float threshold, int thresholdLength){

	notes.clear();
	originalNotes.clear();

	findNotesInImpulsDistribution(threshold, thresholdLength);

	subtractNotesFromSpectralArray();

	notesChanged = true;

	

}


void Editor::findNotesInImpulsDistribution(float threshold, int thresholdLength) {

	int noteNumber = 0;

    for (int f = 1; f < arrayOfComponents[0].numberOfKs; f++) {
        for (int t = 0; t < numberOfSpectralSamples; t++) {
            if (arrayOfComponents[0].impulsDistribution[f][t] > threshold) {

                bool isNote = true;

                for (int k = t + 1; k < t + thresholdLength - 1; k++)
                    if (arrayOfComponents[0].impulsDistribution[f][k] < threshold)
                        isNote = false;



                if (isNote) {
                    bool searchOn = true;
                    int noteLength = thresholdLength - 1;
                    while (searchOn && t + noteLength < numberOfSpectralSamples) {
                        if (arrayOfComponents[0].impulsDistribution[f][t + noteLength] < threshold)
                            searchOn = false;
                        noteLength++;
                    }

                    Note *newNote = new Note(f, t, t + noteLength, noteNumber);
                    notes.add(newNote);
                    t = t + noteLength;
					noteNumber++;
                }
            }
        }
    }
	for(int i = 0; i < notes.size(); i++){

		Note* newNote = new Note(*notes[i]);		
		originalNotes.add(newNote); //muss geändert werden LEAK!
	}
}
/*
void Editor::printNotesToConsole() {

    if (!notes.empty()) {
        for (int k = 0; k < notes.size(); k++) {
            notes[k]->printNote();
        }
    }
}
*/
void Editor::subtractNotesFromSpectralArray() {


    calculateImpulseDistributionForNotes(); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    calculateSpectogramForNotes();

    oneDimensionalSpectralArrayNotes = new fftw_real[sizeOfSpectralArray];

    transform2DFDTo1D(oneDimensionalSpectralArrayNotes);

	//subtraction***********

    for (int i = 0; i < sizeOfSpectralArray; i++) {
        
            spectralArray[i] = spectralArray[i] - (1/backgroundGain) * oneDimensionalSpectralArrayNotes[i]; // hier weitermachen!!!!!!!!!!!!!!!!!

    }
	//*************************

    //delete oneDimensionalSpectralArrayNotes;

}

void Editor::calculateSpectogramForNotes() {

    for (int i = 0; i < nS->numberOfFrequencies; i++)
        for (int k = 0; k < nS->numberOfSamples; k++)
            spectogramFromNotes[i][k] = 0;


    int kmin;
    int kmax;
    float innerSum = 0;

    for (int f = 1; f < nS->numberOfFrequencies; f++) {
        for (int t = 0; t < nS->numberOfSamples; t++) {

            //first sum
            for (int fk = 1; fk < nS->numberOfKernelFrequencies; fk++) {

                //calculates the kmin and kmax for the approximation of P
                kmin = ceil(nS->KNULL - 0.5 + 12 * (nS->logFMinus[f] - nS->logFK[fk]));
                kmax = floor(nS->KNULL + 0.5 + 12 * (nS->logFPlus[f] - nS->logFK[fk]));


                float fplus = (f + 0.5) / fk;
                float fminus = (f - 0.5) / fk;

                if (kmin > 0 && kmin < nS->numberOfKs && kmax > 0 && kmax < nS->numberOfKs) {

                    //inner sum
                    for (int k = kmin; k <= kmax; k++) {
                        float oneDivided = (float) 1 / 24;
                        float minusOneDivided = (float) - 1 / 24;
                        float a = powf(2, oneDivided);
                        float b = powf(2, minusOneDivided);
                        float lamdaKTimesLamdaPlus = nS->lamdaKForK(k) * a;
                        float lamdaKTimesLamdaMinus = nS->lamdaKForK(k) * b;
                        float minimum = (min(lamdaKTimesLamdaPlus, fplus) - max(lamdaKTimesLamdaMinus, fminus));

                        if (minimum > 0) {

							if(backgroundSoundIsActive)
                            innerSum += arrayOfComponents[0].impulsDistribution[k][t] * minimum;
							else
								innerSum += impulseDistributionFromNotes[k][t] * minimum;
                        }
                    }
                }

                spectogramFromNotes[f][t] += arrayOfComponents[0].kernelDistribution[fk] * innerSum;



                innerSum = 0;
            }

        }
    }

	//create Image of Spectogram
	//find highest value

	float highestValue = 0;

	for(int t = 0; t < nS->numberOfSamples; t++){
		for(int f = 0; f < nS->numberOfFrequencies; f++){
			if(spectogramFromNotes[f][t] > highestValue)
				highestValue = spectogramFromNotes[f][t];
		}
	}
	float gainFactor = 255 / highestValue;

	//*******************
	if(imageSpectogramFromNotes != 0)
		delete imageSpectogramFromNotes;

	imageSpectogramFromNotes = new Image(Image::PixelFormat::RGB, nS->numberOfSamples, nS->numberOfFrequencies, true);

	for(int t = 0; t < nS->numberOfSamples; t++){
		for(int f = 0; f < nS->numberOfFrequencies; f++){
			imageSpectogramFromNotes->setPixelAt(t, nS->numberOfFrequencies-1-f, Colour((uint8)0, (uint8)(int)(spectogramFromNotes[f][t]*gainFactor), (uint8)0));
		}
	}

	//**************************************************************************************************

}

void Editor::calculateImpulseDistributionForNotes() {



    for (int i = 0; i < nS->numberOfKs; i++)
        for (int k = 0; k < nS->numberOfSamples; k++)
            impulseDistributionFromNotes[i][k] = 0;

    for (int i = 0; i < notes.size(); i++) {
        for (int k = notes[i]->timeCoordinateStart; k <= notes[i]->timeCoordinateEnd; k++)
            impulseDistributionFromNotes[notes[i]->frequencyCoordinate][k] = arrayOfComponents[0].impulsDistribution[originalNotes[i]->frequencyCoordinate][k];

    }


	//create Image of ImpulseDist
	//find highest value

	float highestValue = 0;

	for(int t = 0; t < nS->numberOfSamples; t++){
		for(int f = 0; f < nS->numberOfKs; f++){
			if(impulseDistributionFromNotes[f][t] > highestValue)
				highestValue = impulseDistributionFromNotes[f][t];
		}
	}
	float gainFactor = 255 / highestValue;

	//*******************
	if(imageImpulseDistFromNotes != 0)
		delete imageImpulseDistFromNotes;

	imageImpulseDistFromNotes = new Image(Image::PixelFormat::RGB, nS->numberOfSamples, nS->numberOfKs, true);

	for(int t = 0; t < nS->numberOfSamples; t++){
		for(int f = 0; f < nS->numberOfKs; f++){
			imageImpulseDistFromNotes->setPixelAt(t, nS->numberOfKs-1-f, Colour((uint8)0,(uint8)(int)(impulseDistributionFromNotes[f][t]*gainFactor),(uint8)0));
		}
	}

	//**************************************************************************************************

}

void Editor::transform2DFDTo1D(fftw_real* spectralArrayNotes) {


    for (int i = 0; i < sizeOfSpectralArray; i++)
        spectralArrayNotes[i] = 0;


	//*************** zero frequency to 0.5
	//for (int i = 0; i < numberOfSpectralSamples; i++) {
	//	spectralArrayNotes[i * lenghtOfFourierTransform] = 0;
	//}

	//*************


    for (int i = 0; i < numberOfSpectralSamples; i++) { // i = 1 changed!
        for (int k = 1; k < lenghtOfFourierTransform / 2; k++) {

			//calcualte original Phase 

			double origPhase = atan(originalSpectralArray[i * lenghtOfFourierTransform + 2 * k + 1] / originalSpectralArray[i * lenghtOfFourierTransform + 2 * k]);

			//------------------
			//magnitude original
			// enable commented part to add merge with original signal. original signal must be subtracted by notes!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			double mangitudeSpectralArray = 0;//sqrt(pow(spectralArray[i * lenghtOfFourierTransform + 2 * k], 2.) + pow(spectralArray[i * lenghtOfFourierTransform + 2 * k + 1], 2.));

			spectralArrayNotes[i * lenghtOfFourierTransform + 2 * k] = (spectogramFromNotes[k][i] + mangitudeSpectralArray) * cos(origPhase);
			spectralArrayNotes[i * lenghtOfFourierTransform + 2 * k+1] = (spectogramFromNotes[k][i] + mangitudeSpectralArray) * sin(origPhase);
        }
    }
}

void Editor::changeExistingNote(int numberOfNote, int newK){

	for(int i = 0; i < notes.size(); i++){
		if(notes[i]->noteNumber == numberOfNote){
			notes[i]->frequencyCoordinate = newK;
			//notesChanged = true;
			break;
		}
	}

	calculateImpulseDistributionForNotes();

	calculateSpectogramForNotes();

	transform2DFDTo1D(oneDimensionalSpectralArrayNotes);

	if(backgroundSoundIsActive){

		for (int i = 0; i < sizeOfSpectralArray; i++) {
			oneDimensionalSpectralArrayForOutput[i] = oneDimensionalSpectralArrayNotes[i] ;//+ backgroundGain * spectralArray[i];
		}
	}
	else{

		for (int i = 0; i < sizeOfSpectralArray; i++) {
			oneDimensionalSpectralArrayForOutput[i] = oneDimensionalSpectralArrayNotes[i];
		}
	}

	Stft stft;

	for(int i = 0; i < lengthOfInputArray; i++) timeDomainSignalForOutput[i] = 0;

	stft.doIstft(oneDimensionalSpectralArrayForOutput, window, timeDomainSignalForOutput, sizeOfSpectralArray -lenghtOfFourierTransform, lenghtOfFourierTransform, 256);

	int biggestEntry = 0;
	for(int i = 1; i < lengthOfInputArray; i++)
		if(timeDomainSignalForOutput[i] > timeDomainSignalForOutput[biggestEntry])
			biggestEntry = i;

	float gain = 0.999 / timeDomainSignalForOutput[biggestEntry];

	for(int i = 0; i < lengthOfInputArray; i++)
		timeDomainSignalForOutput[i] = timeDomainSignalForOutput[i] * gain;

	//***************test
	//timeDomainSignalForOutput = soundFramesArray;
	//****************

	if(originalStream == 0)
		originalStream = new outputStream(timeDomainSignalForOutput, lengthOfInputArray);
	else
		originalStream->addNewAudioData(timeDomainSignalForOutput, lengthOfInputArray);

	if(resampledStream == 0)
		resampledStream = new ResamplingAudioSource(originalStream, false, 1);

}
	

void Editor::getAudioSamplesToPlay(AudioSampleBuffer& buffer, float positionInBar, int samplerate, int numberOfSamples) {

	if(!isLoadingData){
    if (notesChanged) {

		
        calculateImpulseDistributionForNotes();

        calculateSpectogramForNotes();

        transform2DFDTo1D(oneDimensionalSpectralArrayNotes);

		//******************************test notes abschalten
		/*for (int i = 0; i < sizeOfSpectralArray; i++) {
			oneDimensionalSpectralArrayNotes[i] = 0;
		}*/


		//********************************


		if(backgroundSoundIsActive){

			for (int i = 0; i < sizeOfSpectralArray; i++) {
				oneDimensionalSpectralArrayForOutput[i] = oneDimensionalSpectralArrayNotes[i];// + backgroundGain* spectralArray[i];
			}
		}
		else{

			for (int i = 0; i < sizeOfSpectralArray; i++) {
				oneDimensionalSpectralArrayForOutput[i] = oneDimensionalSpectralArrayNotes[i];
			}
		}

		Stft stft;

		for(int i = 0; i < lengthOfInputArray; i++) timeDomainSignalForOutput[i] = 0;

		stft.doIstft(oneDimensionalSpectralArrayForOutput, window, timeDomainSignalForOutput, sizeOfSpectralArray -lenghtOfFourierTransform, lenghtOfFourierTransform, 256);

		int biggestEntry = 0;
		for(int i = 1; i < lengthOfInputArray; i++)
			if(timeDomainSignalForOutput[i] > timeDomainSignalForOutput[biggestEntry])
				biggestEntry = i;

		float gain = 0.999 / timeDomainSignalForOutput[biggestEntry];

		for(int i = 0; i < lengthOfInputArray; i++)
			timeDomainSignalForOutput[i] = timeDomainSignalForOutput[i] * gain;

		//***************test
		//timeDomainSignalForOutput = soundFramesArray;
		//****************

		if(originalStream == 0)
			originalStream = new outputStream(timeDomainSignalForOutput, lengthOfInputArray);
		else
			originalStream->addNewAudioData(timeDomainSignalForOutput, lengthOfInputArray);

		if(resampledStream == 0)
			resampledStream = new ResamplingAudioSource(originalStream, false, 1);

		notesChanged = false;

    }

	resampledStream->prepareToPlay(numberOfSamples, samplerate);

	resampledStream->setResamplingRatio((double)inputSamplerate / samplerate);

	AudioSourceChannelInfo drStrange(buffer);

	resampledStream->getNextAudioBlock(drStrange);

	}

}

void Editor::loadExistingData(const char* fileName){

	//create hanning window
    /*window = new fftw_real[lenghtOfFourierTransform];
    for (int i = 0; i < lenghtOfFourierTransform; i++)
        window[i] = 0.5 - 0.5 * cos(6.283 * ((float) i / (lenghtOfFourierTransform - 1)));*/
	isLoadingData = true;

	arrayOfComponents = new MskComponent[1];
	arrayOfComponents[0] = *(new MskComponent(fileName));

	//reconstruct length from loaded Component
	int newLengthOfTimeInput = arrayOfComponents[0].sizeOfInput;
	lengthOfInputArray = newLengthOfTimeInput;
	int newLengthOfSpectralArray = 1024 * (ceilf(newLengthOfTimeInput / 256)+1 );
	sizeOfSpectralArray = newLengthOfSpectralArray;
	numberOfSpectralSamples = arrayOfComponents[0].numberOfSamples;
	
	inputSamplerate = arrayOfComponents[0].inputSamplerate;


	soundFramesArray = arrayOfComponents[0].timeDomainInput;

	timeDomainSignalForOutput = new fftw_real[newLengthOfTimeInput];

	spectralArray = new fftw_real[newLengthOfSpectralArray];

	transformToFD(soundFramesArray, spectralArray, newLengthOfTimeInput, 256);

	originalSpectralArray = new fftw_real[newLengthOfSpectralArray];

	//copy spectralArray
	for(int i = 0; i < sizeOfSpectralArray; i++){
		originalSpectralArray[i] = spectralArray[i];
	}
	//-------

    oneDimensionalSpectralArrayForOutput = new fftw_real[newLengthOfSpectralArray];

	nS = new NoteSeparator(1, arrayOfComponents[0].numberOfKernelFrequencies, arrayOfComponents[0].numberOfKs, lenghtOfFourierTransform / 2, arrayOfComponents[0].numberOfSamples, spectogram, containingThread);

	impulseDistributionFromNotes = new float*[nS->numberOfKs];
    for (int i = 0; i < nS->numberOfKs; ++i)
        impulseDistributionFromNotes[i] = new float[nS->numberOfSamples];

    spectogramFromNotes = new float*[nS->numberOfFrequencies];
    for (int i = 0; i < nS->numberOfFrequencies; ++i)
        spectogramFromNotes[i] = new float[nS->numberOfSamples];

	findNotesInImpulsDistribution(0.0000001, 3);

	subtractNotesFromSpectralArray();

	isLoadingData = false;
	notesChanged = true;

	
}



