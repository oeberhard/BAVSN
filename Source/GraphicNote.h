/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  22 Jan 2013 12:26:14pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_GRAPHICNOTE_GRAPHICNOTE_E5BED4E__
#define __JUCER_HEADER_GRAPHICNOTE_GRAPHICNOTE_E5BED4E__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GraphicNote  : public Component
{
public:
    //==============================================================================
    GraphicNote (float new_heightOfSemitone, int new_noteNumber, NewProjectAudioProcessor* new_ownerFilter);
    ~GraphicNote();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
	void mouseDrag (const MouseEvent& event);
	void mouseDown(const MouseEvent& event);
	void mouseUp(const MouseEvent& event);
	int noteNumber;


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
	float heightOfSemitone;
	int originalX;
	int originalY;
	NewProjectAudioProcessor* ownerFilter;
    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    GraphicNote (const GraphicNote&);
    const GraphicNote& operator= (const GraphicNote&);
};


#endif   // __JUCER_HEADER_GRAPHICNOTE_GRAPHICNOTE_E5BED4E__
