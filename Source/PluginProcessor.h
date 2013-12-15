/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __PLUGINPROCESSOR_H_C2C2EDB__
#define __PLUGINPROCESSOR_H_C2C2EDB__

#include "../JuceLibraryCode/JuceHeader.h"
#include "LoadNAnalyseFileThread.h"
#include "Editor.h"
//#include "PluginEditor.h"
//#include "SequencerWindow.h"



//==============================================================================
/**
*/
class NewProjectAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

	virtual bool silenceInProducesSilenceOut() const;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

	ActionBroadcaster* newNotesFromAnalyser;

	void changeNote(int numberOfNote, Point<int> newPosition, Point<int> dimensionOfSequencer);
	void noteThresholdChanged(float threshold, int thresholdLength);

	bool analyseNewFile;
	bool editorIsReady;
	bool noteThresholdIsChanged;

	Editor* soundEditor;

	void loadExistingData();
	

private:
	bool loadingNewComponent;
	int currentSamplesPerBlock;
    //==============================================================================
	
	

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor);
};

#endif  // __PLUGINPROCESSOR_H_C2C2EDB__
