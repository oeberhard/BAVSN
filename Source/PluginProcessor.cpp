/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//#include "stdafx.h"
#include <windows.h>
//#include "resource.h"
//#include <Commdlg.h>


//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
{
	newNotesFromAnalyser = new ActionBroadcaster();
	editorIsReady = false;
	analyseNewFile = false;
	soundEditor = 0;
	noteThresholdIsChanged = false;
	loadingNewComponent = false;
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//==============================================================================

//====================================================== custom Methods

void* loadNewWaveFile(void* fakeThis){

		NewProjectAudioProcessor* realThis = (NewProjectAudioProcessor*)fakeThis;

		FileChooser myChooser ("Please select the wave file you want to load...",
		File::getSpecialLocation (File::userHomeDirectory),
		"*.wav");


		if (myChooser.browseForFileToOpen())
		{
			File waveFile (myChooser.getResult());

			LoadNAnalyseFileThread task(waveFile, realThis);

			task.runThread();

		}
		
		void* dummy = 0;

		return dummy;
}

//============================================================================
const String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int NewProjectAudioProcessor::getNumParameters()
{
    return 0;
}

float NewProjectAudioProcessor::getParameter (int index)
{
    return 0.0f;
}

void NewProjectAudioProcessor::setParameter (int index, float newValue)
{
}

const String NewProjectAudioProcessor::getParameterName (int index)
{
    return String::empty;
}

const String NewProjectAudioProcessor::getParameterText (int index)
{
    return String::empty;
}

const String NewProjectAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String NewProjectAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool NewProjectAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool NewProjectAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool NewProjectAudioProcessor::silenceInProducesSilenceOut() const
{
	return false;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 0;
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const String NewProjectAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void NewProjectAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

		currentSamplesPerBlock = samplesPerBlock;


}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void NewProjectAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{

	if(editorIsReady)
		newNotesFromAnalyser->sendActionMessage("N");


	if(analyseNewFile){

		analyseNewFile = false;

		MessageManager* mm = MessageManager::getInstance();


		void* dummy = this;

		void* d = mm->callFunctionOnMessageThread(loadNewWaveFile, dummy);
	}







    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < getNumInputChannels(); ++channel)
    {


    }

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

	AudioPlayHead* playHead = getPlayHead();

	AudioPlayHead::CurrentPositionInfo info;

	playHead->getCurrentPosition(info);

	float* channelData = buffer.getSampleData (0);
	if(soundEditor != 0 && !loadingNewComponent)
		soundEditor->getAudioSamplesToPlay(buffer, info.ppqPositionOfLastBarStart, getSampleRate(), currentSamplesPerBlock);
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}


void NewProjectAudioProcessor::changeNote(int numberOfNote, Point<int> newPosition, Point<int> dimensionOfSequencer){

	float heightOfNote = dimensionOfSequencer.getY() / this->soundEditor->nS->numberOfKs;
	int newK = (dimensionOfSequencer.getY() - newPosition.getY())/ heightOfNote;

	if(newK == 33)
		newK = 32;
	if(newK < 1)
		newK = 1;

	soundEditor->changeExistingNote(numberOfNote, newK-1);//edit 4. märz
	
		

	newNotesFromAnalyser->sendActionMessage("G");

}

void NewProjectAudioProcessor::noteThresholdChanged(float threshold, int thresholdLength){

	soundEditor->findNotesWithOtherParameters(threshold, thresholdLength);
	//noteThresholdIsChanged = true;
	newNotesFromAnalyser->sendActionMessage("T");
	newNotesFromAnalyser->sendActionMessage("G");
}

void NewProjectAudioProcessor::loadExistingData(){

	loadingNewComponent = true;

	LoadNAnalyseFileThread* thread = 0;
	soundEditor = new Editor(1024, thread);

	OPENFILENAME ofn ;
	// a another memory buffer to contain the file name
	char szFile[100] ;

	// open a file name
	ZeroMemory( &ofn , sizeof( ofn));
	ofn.lStructSize = sizeof ( ofn );
	ofn.hwndOwner = NULL  ;
	ofn.lpstrFile = szFile ;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof( szFile );
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex =1;
	ofn.lpstrFileTitle = NULL ;
	ofn.nMaxFileTitle = 0 ;
	ofn.lpstrInitialDir=NULL ;
	ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
	if(GetOpenFileName( &ofn )){


	soundEditor->loadExistingData(ofn.lpstrFile);
	editorIsReady = true;
	loadingNewComponent = false;
	}
}
