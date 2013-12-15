#pragma once

#include <windows.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "rfftw.h"
#include "fftw.h"
//#include "PluginProcessor.h"


class NewProjectAudioProcessor;

class Editor;

class LoadNAnalyseFileThread : public ThreadWithProgressWindow
{
public:
	LoadNAnalyseFileThread(File new_waveFile, NewProjectAudioProcessor* new_audioProcessor) : ThreadWithProgressWindow ("Analysis in progress (may take 30 min)...", true, false){
		waveFile = new_waveFile;
		audioProcessor = new_audioProcessor;
		
	}


	void run();
	//Editor* getEditor();

private:
	void analyseFile(float noteDetectionThreshold, int noteDetectionThresholdLength);
	const char* loadFile();

	NewProjectAudioProcessor* audioProcessor;
	File waveFile;
	Component* plugEditor;

	
};

