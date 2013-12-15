/*
 * NoteSeparator.h
 *
 *  Created on: 19.09.2012
 *      Author: baam
 */

#ifndef NOTESEPARATOR_H_
#define NOTESEPARATOR_H_
#include <cmath>
#include "MskComponent.h"
#include <iostream> 
class LoadNAnalyseFileThread;
//#include <assert.h>

using namespace std;

class NoteSeparator {
public:
    NoteSeparator(int new_numberOfComponents, int new_numberOfKernelFrequencies, int numberOfKs, 
            int new_numberOfFrequencies, int new_numberOfSamples, float** new_spectogram, LoadNAnalyseFileThread* thread);
    virtual ~NoteSeparator();

public:

    int numberOfComponents;
    int numberOfKernelFrequencies;
    int numberOfSamples;
    int numberOfFrequencies;
    int numberOfKs;
    static const int KNULL = 26;//16;
    float epsilon;
    //static const float DELTA = 0.9999999999; // for ceilingfunction
    float ** spectogram;
    MskComponent* components;
    float* logFPlus;
    float* logFMinus;
    float* logFK;
	LoadNAnalyseFileThread* containigThread;

    //double roots[12];
    //int indexes[17];

    //Used by Updaterule 3


    MskComponent* updateCycle(int numberOfIterations, int numberOfFixedPointIterations);
    void updatesPZOfAllComponents(MskComponent updatedComponents[], float **currentApprox2);
    void updatesKernelDistributionOfAllComponents(MskComponent updatedComponents[], float **currentApprox2);
    //void updatesImpulsDistributionOfAllComponents(Component components[]);
    void updatesImpulseDistributionOfAllComponents(MskComponent updatedComponents[], float **currentApprox2, float **newImpulsDistribution2, int numberOfFixedPointIterations);
    float updateImpulseDistributionOfComponent(MskComponent &component, int kL, int tGivenZ, float **currentApprox2, float **newImpulsDistribution2);
    float updatesKernelDistributionOfComponent(MskComponent &component, int fk, float **currentApprox2);
    float updatesPZOfComponent(MskComponent &component, float **currentApprox2);
    void updatesCurrentApproximation(float **currentApprox2);
    float lamdaKForK(int k);
    void printApproximationToFileAndBmp(float** currentApprox2);
    //int Ceil12TimesLog2(double x, bool plusMinus);

    //void loadData(float newData[numberOfFrequencies][numberOfSamples]);

};

#endif /* NOTESEPARATOR_H_ */
