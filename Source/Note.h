/* 
 * File:   Note.h
 * Author: Boom
 *
 * Created on 28. November 2012, 19:09
 */

#ifndef NOTE_H
#define	NOTE_H

#include <iostream>

class Note {
public:
    Note(int new_frequencyCoordinate, int start, int end, int new_noteNumber);
    Note(const Note& orig);
    virtual ~Note();
    void printNote();

    int frequencyCoordinate;
    int timeCoordinateStart;
    int timeCoordinateEnd;
	int noteNumber;
private:

};

#endif	/* NOTE_H */

