/* 
 * File:   Note.cpp
 * Author: Boom
 * 
 * Created on 28. November 2012, 19:09
 */

#include "Note.h"

Note::Note(int new_frequencyCoordinate, int start, int end, int new_noteNumber) {
    frequencyCoordinate = new_frequencyCoordinate;
    timeCoordinateStart = start;
    timeCoordinateEnd = end;
	noteNumber = new_noteNumber;
    
}

Note::Note(const Note& orig) {

	frequencyCoordinate = orig.frequencyCoordinate;
	timeCoordinateStart = orig.timeCoordinateStart;
	timeCoordinateEnd = orig.timeCoordinateEnd;
	noteNumber = orig.noteNumber;
}

Note::~Note() {
}

void Note::printNote(){
    std::cout << "Frequency: " << frequencyCoordinate << "  Begin: " <<  timeCoordinateStart << " End: " << timeCoordinateEnd << std::endl;
}

