/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __PLUGINEDITOR_H_3CE811B8__
#define __PLUGINEDITOR_H_3CE811B8__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "SequencerWindow.h"
#include "Editor.h"
#include "GraphicNote.h"


//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public AudioProcessorEditor, public ActionListener, public Slider::Listener, public ButtonListener
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor* ownerFilter);
    ~NewProjectAudioProcessorEditor();

	void actionListenerCallback	(	const String & 	message	);
	void sliderValueChanged (Slider* slider);


    //==============================================================================
    // This is just a standard Juce paint method...
    void paint (Graphics& g);
	
	bool notesAreSetUp;

	void buttonClicked (Button* button);
private:

	Image* drawKernelDistribution();

	Array<GraphicNote*> graphicNotes;
	SequencerWindow* sequencer;
	NewProjectAudioProcessor* audioProcessor;
	Slider* slider;
	Slider* backgroundGain;
	TextButton* buttonAnalyse;
	TextButton* buttonLoad;
	ToggleButton* buttonBackgroundSound;
	ImageComponent* spectrumFromNotes;
	ImageComponent* impulsDistFromNotes;
	ImageComponent* kernelDistributionImage;
	Image* imageKD;
	bool buttonsExpired;
};


#endif  // __PLUGINEDITOR_H_3CE811B8__
