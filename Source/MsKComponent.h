/* 
 * File:   Component.h
 * Author: Boom
 *
 * Created on 6. November 2012, 16:53
 */

#ifndef COMPONENT_H
#define	COMPONENT_H
#include <cstdlib>
#include "EasyBMP.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "rfftw.h"
#include "fftw.h"
#include <cmath>
#include <sstream>
using namespace std;

class MskComponent {
public:
    MskComponent();
    MskComponent(int numberOfKs, int numberOfKernelFrequencies, int numberOfSamples);
    MskComponent(const char* fileName);
    
    MskComponent(const MskComponent& orig);
    virtual ~MskComponent();

    void initializeComponent();
    void printImpulseDistributionToFile();
    void writeComponentToFile(double* timeDomainInput, int timeDomainInputSize, int samplerate);
    //component represents one tone model and holds its probability P(z), kernel distribution and impulse distribution


    float probabilityComponent;
    float *kernelDistribution;
    float **impulsDistribution;
    int numberOfKs;
    int numberOfKernelFrequencies;
    int numberOfSamples;
	double* timeDomainInput;
	int sizeOfInput;
	int inputSamplerate;
private:

};

#endif	/* COMPONENT_H */

