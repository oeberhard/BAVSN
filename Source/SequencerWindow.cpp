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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "SequencerWindow.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SequencerWindow::SequencerWindow (int new_numberOfSemitones/*, int numberOfSpectralSamples*/, int new_pathDistanceToOutline/*, Array<Note*> mskNotes*/)
{

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 300);
	numberOfSemitones = new_numberOfSemitones;
	pathDistanceToOutline = new_pathDistanceToOutline;

	heightOfCell = getHeight() / numberOfSemitones;
	lengthOfCell = getWidth() - 2 * new_pathDistanceToOutline;

	for(int i=1; i<numberOfSemitones -1; i++){
		Path* newPath = new Path();
		newPath->startNewSubPath(pathDistanceToOutline, heightOfCell * i);
		newPath->lineTo(getWidth() - pathDistanceToOutline, heightOfCell * i);
		gridPaths.add(newPath);
	}

	/*for(int i=0; i<mskNotes.size(); i++){
		GraphicNote* new_gNote = new GraphicNote(mskNotes[i]->frequencyCoordinate, mskNotes[i]->timeCoordinateEnd - mskNotes[i]->timeCoordinateStart,
			mskNotes[i]->timeCoordinateStart, heightOfCell, lengthOfCell);
		
		addAndMakeVisible(new_gNote);

	}*/

	




    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SequencerWindow::~SequencerWindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

	for(int i=0; i<gridPaths.size(); i++)
		delete gridPaths[i];


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SequencerWindow::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

  /*  g.fillAll (Colours::black);

    g.setColour (Colours::white);
    g.fillPath (internalPath1);
    g.setColour (Colours::white);*/
	g.setColour (Colours::white);
	g.setOpacity (1.0f);
	for(int i=0; i<gridPaths.size(); i++){
		/*g.setColour (Colours::white);
		g.fillPath (internalPath1);*/
		
		g.strokePath(*gridPaths[i], PathStrokeType (0.100f));
	}

	

    
    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SequencerWindow::resized()
{
    internalPath1.clear();
    internalPath1.startNewSubPath ((float) (getWidth() - 10), 10.0f);
    internalPath1.lineTo (10.0f, 10.0f);
    internalPath1.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}





//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SequencerWindow" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="0" initialWidth="600" initialHeight="300">
  <BACKGROUND backgroundColour="ffffffff">
    <PATH pos="0 0 100 100" fill="solid: ff2aa586" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff2a32a5" nonZeroWinding="1">s 10R 10 l 10 10 x</PATH>
  </BACKGROUND>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
