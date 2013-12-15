/* 
 * File:   Editor.h
 * Author: Boom
 *
 * Created on 22. Oktober 2012, 21:28
 */

#ifndef EDITOR_H
#define	EDITOR_H
//#include <sndfile.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include "rfftw.h"
#include "fftw.h"
#include "Stft.h"
#include <cmath>
#include <iostream> 
#include "MskComponent.h"
#include "NoteSeparator.h"
#include "Note.h"
#include "LoadNAnalyseFileThread.h"
#include "EasyBMP.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "outputStream.h"
#include <ctime>




class Editor {
public:
    Editor(int new_numberOfFrequencyBins, LoadNAnalyseFileThread* thread);
    Editor(const Editor& orig);
    virtual ~Editor();
    void loadWaveFile(File new_waveFile);
    void transformToFD(fftw_real *inputArray, fftw_real* outputArray, int sizeOfInput, int hopsize);
    MskComponent* separateNotes(int numberOfComponents, int numberOfKernelFrequencies, int numberOfKs, int numberOfIterations, int numberOfFixedPointIterations);
    void separateNotesFromFile(File waveFile, int numberOfComponents, int numberOfKernelFrequencies, int numberOfKs, int numberOfIterations, int numberOfFixedPointIterations);
    void printSpectogramToFile();
   // void printNotesToConsole();
    void findNotesInImpulsDistribution(float threshold, int thresholdLength);
    void subtractNotesFromSpectralArray();
    MskComponent* arrayOfComponents;
    void getAudioSamplesToPlay(AudioSampleBuffer& buffer, float positionInBar, int samplerate, int numberOfSamples);

	void loadExistingData(const char* fileName);
    
    int numberOfKFrequencies;
	int sizeOfSpectralArray;
	Array<Note*> notes;
	Array<Note*> originalNotes;
	int numberOfSpectralSamples;
	NoteSeparator* nS;

	void changeExistingNote(int numberOfNote, int newK);
	void findNotesWithOtherParameters(float threshold, int thresholdLength);

	
	Image* imageSpectogramFromNotes;
	Image* imageImpulseDistFromNotes;

	bool backgroundSoundIsActive;
	bool notesChanged;
	double* soundFramesArray;
	int lengthOfInputArray;
	AudioFormatReader* _audioFormatReader;
	float backgroundGain;

private:
    void transformToSpectogram(fftw_real* spectralArrayFromFttw);
    void calculateSpectogramForNotes();
    void calculateImpulseDistributionForNotes();
    void transform2DFDTo1D(fftw_real* spectralArrayNotes);
    
    
    fftw_real* spectralArray;
	fftw_real* originalSpectralArray;
    
//    SF_INFO *soundFileInfo; 
    float** spectogram;
    float** impulseDistributionFromNotes;
    float** spectogramFromNotes;
    fftw_real* oneDimensionalSpectralArrayNotes;
    fftw_real* oneDimensionalSpectralArrayForOutput;
    
    int lenghtOfFourierTransform;
    
    
	LoadNAnalyseFileThread* containingThread;

	
	fftw_real* timeDomainSignalForOutput;

	fftw_real* window;
	outputStream* originalStream;
	ResamplingAudioSource* resampledStream;
	int inputSamplerate;
	bool isLoadingData;
    
	

	
	
	
    

    
    
    
};

#endif	/* EDITOR_H */

