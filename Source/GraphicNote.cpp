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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "GraphicNote.h"
#include "PluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GraphicNote::GraphicNote (float new_heightOfSemitone, int new_noteNumber, NewProjectAudioProcessor* new_ownerFilter)
{

    //[UserPreSize]
    //[/UserPreSize]

	setSize(10,10);
    //setSize (lengthOfCell * lengthOfNote, heightOfCell);
	//setTopLeftPosition((timePositionOfNote + 1) * lengthOfNote * lengthOfCell, heightOfCell * frequencyBin);
	heightOfSemitone = new_heightOfSemitone;
	originalX = 0;
	originalY = 0;
	noteNumber = new_noteNumber;
	ownerFilter = new_ownerFilter;
    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

GraphicNote::~GraphicNote()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GraphicNote::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour(0, (uint8)160, 0));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GraphicNote::resized()
{
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GraphicNote::mouseUp(const MouseEvent& event){

	Point<int> sequencerDimension;
	sequencerDimension.setX(getParentWidth());
	sequencerDimension.setY(getParentHeight());

	//Point<int> finalPosition(originalX +  !!!!!!!!!!!!!!! dragfromstart

	ownerFilter->changeNote(noteNumber, this->getPosition(), sequencerDimension);


}

void GraphicNote::mouseDown(const MouseEvent& event){

	originalX = getX();
	originalY = getY();

}

void GraphicNote::mouseDrag (const MouseEvent& event){
	
	
	int offsetK = event.getDistanceFromDragStartY() / heightOfSemitone;



		setTopLeftPosition(getX(), offsetK * heightOfSemitone + originalY);
}


//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GraphicNote" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff3fe22f"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
