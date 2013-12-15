#include "LoadNAnalyseFileThread.h"
#include "Editor.h"
#include "SequencerWindow.h"
#include "PluginProcessor.h"
//#include "PluginEditor.h"





void LoadNAnalyseFileThread::run(){

	//const MessageManagerLock mmLock;
	//noteEditor = 0;
	//const char* filename = loadFile();
	
	analyseFile(0.0001, 7);
		
}

void LoadNAnalyseFileThread::analyseFile(float noteDetectionThreshold, int noteDetectionThresholdLength) {


	getAlertWindow()->setColour(AlertWindow::backgroundColourId, Colour(0, (uint8)150, 0));

	audioProcessor->soundEditor = new Editor(1024, this);

	audioProcessor->soundEditor->separateNotesFromFile(waveFile, 1, 400, 32, 100, 25);

	audioProcessor->soundEditor->arrayOfComponents[0].printImpulseDistributionToFile();

	audioProcessor->soundEditor->arrayOfComponents[0].writeComponentToFile(audioProcessor->soundEditor->soundFramesArray, audioProcessor->soundEditor->lengthOfInputArray, audioProcessor->soundEditor->_audioFormatReader->sampleRate);

	audioProcessor->soundEditor->findNotesInImpulsDistribution(noteDetectionThreshold, noteDetectionThresholdLength);

	audioProcessor->soundEditor->subtractNotesFromSpectralArray();



	audioProcessor->editorIsReady = true;

	/*for(int i=0; i<soundEditor->notes.size(); i++){
	GraphicNote* new_gNote = new GraphicNote(soundEditor->notes[i]->frequencyCoordinate, soundEditor->notes[i]->timeCoordinateEnd - soundEditor->notes[i]->timeCoordinateStart,
	soundEditor->notes[i]->timeCoordinateStart, 3, 3);

	plugEditor->findChildWithID("Sequencer")->addAndMakeVisible(new_gNote);
	new_gNote->toFront(false);
	new_gNote->setTopLeftPosition(soundEditor->notes[i]->timeCoordinateStart, soundEditor->notes[i]->frequencyCoordinate);

	//setTopLeftPosition((timePositionOfNote + 1) * lengthOfNote * lengthOfCell, heightOfCell * frequencyBin);
	}*/
	//GraphicNote* ng = new GraphicNote();
	//Component* c = plugEditor->findChildWithID("Sequencer");//->addChildComponent(ng);
	//ng->setVisible(true);
	//ng->toFront(true);



	//SequencerWindow* new_sequencer = new SequencerWindow(soundEditor->numberOfKFrequencies, soundEditor->sizeOfSpectralArray, 10/*, soundEditor->notes*/);
	
	

	//dynamic_cast<NewProjectAudioProcessorEditor*>(dynamic_cast<NewProjectAudioProcessor*>(audioProcessor)->getActiveEditor())->sequencer = new_sequencer;;

	//dynamic_cast<NewProjectAudioProcessorEditor*>(dynamic_cast<NewProjectAudioProcessor*>(audioProcessor)->getActiveEditor())->addAndMakeVisible(new_sequencer);
    //noteEditor->separateNotesFromFile(fileName, 1, 512, 32, 1, 2);

	
    

    //editorReady = true;


}

const char* LoadNAnalyseFileThread::loadFile() {

    HWND hwnd;
    OPENFILENAME ofn;
    char szFileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof (ofn));

    ofn.lStructSize = sizeof (ofn); // SEE NOTE BELOW
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Wave Files (*.wav)\0*.wav\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "wav";

    if (GetOpenFileName(&ofn)) {
        return szFileName;
    }
	else
		return "";

}
/*
Editor* LoadNAnalyseFileThread::getEditor(){
	if(noteEditor != 0)
		return noteEditor;
}*/