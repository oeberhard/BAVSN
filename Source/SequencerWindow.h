/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  21 Jan 2013 11:49:18am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_SEQUENCERWINDOW_SEQUENCERWINDOW_438E9FD0__
#define __JUCER_HEADER_SEQUENCERWINDOW_SEQUENCERWINDOW_438E9FD0__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
//#include <vector>
#include "Note.h"
#include "GraphicNote.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SequencerWindow  : public Component
{
public:
    //==============================================================================
    SequencerWindow (int new_numberOfSemitones/*, int numberOfSpectralSamples*/, int new_pathDistanceToOutline/*, Array<Note*> mskNotes*/);
    ~SequencerWindow();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
	
	


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    Path internalPath1;
	Array<Path*> gridPaths;
	//Array<GraphicNote*> gNotes;
	int numberOfSemitones;
	int pathDistanceToOutline;
	int heightOfCell;
	int lengthOfCell;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    SequencerWindow (const SequencerWindow&);
    const SequencerWindow& operator= (const SequencerWindow&);
};


#endif   // __JUCER_HEADER_SEQUENCERWINDOW_SEQUENCERWINDOW_438E9FD0__
