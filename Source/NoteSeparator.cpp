/*
 * NoteSeparator.cpp
 *
 *  Created on: 19.09.2012
 *      Author: baam
 */

#define LAMDA = pow(2-KNULL,1/12);

#include "NoteSeparator.h"
#include "LoadNAnalyseFileThread.h"

NoteSeparator::NoteSeparator(int new_numberOfComponents, int new_numberOfKernelFrequencies, int new_numberOfKs, 
        int new_numberOfFrequencies, int new_numberOfSamples, float** new_spectogram, LoadNAnalyseFileThread* thread) {

	//constants
			epsilon  = 0.0000000001; // to make divisor non zero


    numberOfComponents = new_numberOfComponents;
    numberOfKernelFrequencies = new_numberOfKernelFrequencies;
    spectogram = new_spectogram;
    numberOfSamples = new_numberOfSamples;
    numberOfFrequencies = new_numberOfFrequencies;
    numberOfKs = new_numberOfKs;
	
	containigThread = thread;

    components = new MskComponent[new_numberOfComponents];
    std::fill(components, components + new_numberOfComponents, MskComponent(new_numberOfKs, new_numberOfKernelFrequencies, new_numberOfSamples));
    for (int i = 0; i < new_numberOfComponents; i++) {
        components[i].initializeComponent();
    }

	float logOf2 = logf(2);

    logFPlus = new float[new_numberOfFrequencies];
    for (int i = 1; i < new_numberOfFrequencies; i++)
        logFPlus[i] = logf(i + 0.5)/logOf2;
    logFPlus[0] = 0;

    logFMinus = new float[new_numberOfFrequencies];
    for (int i = 1; i < new_numberOfFrequencies; i++)
        logFMinus[i] = logf(i - 0.5)/logOf2;
    logFMinus[0] = 0;

    logFK = new float[new_numberOfKernelFrequencies];
    for (int i = 1; i < new_numberOfKernelFrequencies; i++)
        logFK[i] = logf(i)/logOf2;
    logFK[0] = 0;



}

NoteSeparator::~NoteSeparator() {
    // TODO Auto-generated destructor stub
}

float NoteSeparator::lamdaKForK(int k) {
    float divK = (float)(k - KNULL) / (float)12;
    return pow(2, divK);
}

/***************************************************************************
 * Update cycle executes the three multiplicative update rules on page 5
 ***************************************************************************/

MskComponent* NoteSeparator::updateCycle(int numberOfIterations, int numberOfFixedPointIterations) {

    float** newImpulsDistribution = new float*[numberOfKs];
    for (int i = 0; i < numberOfKs; ++i)
        newImpulsDistribution[i] = new float[numberOfSamples];

    float** currentApprox = new float*[numberOfFrequencies];
    for (int i = 0; i < numberOfFrequencies; ++i)
        currentApprox[i] = new float[numberOfSamples];

    MskComponent updatedComponents[1];
    std::fill(updatedComponents, updatedComponents + numberOfComponents, MskComponent(numberOfKs, numberOfKernelFrequencies, numberOfSamples));
	//neue Line
	//updatedComponents[0].initializeComponent();
	//--------------

    //this array holds the updated components until all update rules have been applied


    for (int x = 0; x < numberOfIterations; x++) {

		containigThread->setProgress(x/(double)numberOfIterations);

        cout << x << endl;

        //EVENTUELL UPDATEDCOMPONENTS NEU INITIALISIEREN!!!!!!!!!!!!!!!!!!!!!!



        for (int r = 0; r < numberOfFrequencies; r++) {
            for (int c = 0; c < numberOfSamples; c++) {
                currentApprox[r][c] = 0;
            }
        }

        updatesCurrentApproximation(currentApprox);

        //updates P(z) of all components (Update rule 1 Page 5)
        if (numberOfComponents != 1)
            updatesPZOfAllComponents(updatedComponents, currentApprox);
        else
            updatedComponents[0].probabilityComponent = 1;

        //updates Pk(fk|z) of all components (Update rule 2 Page 5)
        updatesKernelDistributionOfAllComponents(updatedComponents, currentApprox);

        //updates Pi(LAMDAk|tz) of all components (Update rule 3 Page 5)
        updatesImpulseDistributionOfAllComponents(updatedComponents, currentApprox, newImpulsDistribution, numberOfFixedPointIterations);

        //copies updatedComponents to real components
        for (int i = 0; i < numberOfComponents; i++) {
            components[i] = updatedComponents[i];
        }
    }

    printApproximationToFileAndBmp(currentApprox);

    return components;

}

/***************************************************************************
 * Updates P(z) (probability of component)
 ***************************************************************************/

void NoteSeparator::updatesPZOfAllComponents(MskComponent updatedComponents[], float **currentApprox2) {

    float sumOfAllPZUpdates;
    float updates[1];

    //sums up all component updates for normalization
    for (int i = 0; i < numberOfComponents; i++) {
        updates[i] = updatesPZOfComponent(components[i], currentApprox2);
        sumOfAllPZUpdates += updates[i];
    }

    //normalizes all component updates by division through sumOfAllComponents
    for (int i = 0; i < numberOfComponents; i++) {
        updatedComponents[i].probabilityComponent = (updates[i] / (sumOfAllPZUpdates + epsilon));
    }
}

/*Update of Parameter P(z) Page 5
 * firstly the sum is calculated and stored in sum, then the multiplication is done
 */

float NoteSeparator::updatesPZOfComponent(MskComponent &component, float **currentApprox2) {

    int kmin;
    int kmax;
    float sum;

    //calculates nested sum
    for (int f = 1; f < numberOfFrequencies; f++) {
        for (int t = 0; t < numberOfSamples; t++) {
            for (int fk = 1; fk < numberOfKernelFrequencies; fk++) {

                //    if (f > fk) {
                //calculates discretisation of lamda
                kmin = ceil(KNULL - 0.5 + 12 * (logFMinus[f] - logFK[fk]));
                kmax = floor(KNULL + 0.5 + 12 * (logFPlus[f] - logFK[fk]));

                //calculates inner sum with lamdaK
                float impuls = 0;

                if (kmin > 0 && kmin < numberOfKs && kmax > 0 && kmax < numberOfKs) {

                    for (int k = kmin; k <= kmax; k++) {

                        float oneDivided = (float) 1 / 24;
                        float minusOneDivided = (float) - 1 / 24;
                        float a = powf(2, oneDivided);
                        float b = powf(2, minusOneDivided);
                        float lamdaKTimesLamdaPlus = lamdaKForK(k) * a;
                        float lamdaKTimesLamdaMinus = lamdaKForK(k) * b;



                        //precalculate values since fmin/fmax dont accept expressions
                        float fplus = (float) (f + 0.5) / fk;
                        float fminus = (float) (f - 0.5) / fk;

                        impuls += component.impulsDistribution[k][t]*(min(lamdaKTimesLamdaPlus, fplus) - max(lamdaKTimesLamdaMinus, fminus));
                    }


                    //putting together the product and add to sum
                    sum += spectogram[f][t] / (currentApprox2[f][t] + epsilon) * component.kernelDistribution[fk] * impuls;

                }
                //         }
            }
        }
    }

    //returns product of current component probability and sum
    return component.probabilityComponent * sum;
}

/***************************************************************************
 * Updates kernel distribution
 ***************************************************************************/

void NoteSeparator::updatesKernelDistributionOfAllComponents(MskComponent updatedComponents[], float **currentApprox2) {

    float sumOfAllKernelFrequencyUpdates = 0;

    float** updates = new float*[numberOfComponents]; // MUSS WEG!!! KEINE ALLOC IN ALGO
    for (int i = 0; i < numberOfComponents; ++i)
        updates[i] = new float[numberOfKernelFrequencies];

    updates[0][0] = 0;

    //float updates[numberOfComponents][numberOfKernelFrequencies];

    //iterates through all components
    for (int i = 0; i < numberOfComponents; i++) {

        for (int fk = 1; fk < numberOfKernelFrequencies; fk++) {

            updates[i][fk] = updatesKernelDistributionOfComponent(components[i], fk, currentApprox2);
            sumOfAllKernelFrequencyUpdates += updates[i][fk];
        }

        for (int fk = 1; fk < numberOfKernelFrequencies; fk++) {
            updatedComponents[i].kernelDistribution[fk] = updates[i][fk] / (sumOfAllKernelFrequencyUpdates + epsilon);
        }

        sumOfAllKernelFrequencyUpdates = 0;
    }

    for (int i = 0; i < numberOfComponents; i++)
        delete[] updates[i];
    delete[] updates;

}

//Update of Parameter Pk(f',z) for a given f'; Page 5

float NoteSeparator::updatesKernelDistributionOfComponent(MskComponent &component, int fk, float **currentApprox2) {

    int kmin;
    int kmax;
    float sum = 0;

    for (int f = 1; f < numberOfFrequencies; f++) {
        for (int t = 0; t < numberOfSamples; t++) {

            //        if (f > fk) {
            //calculates discretisation of lamda
            kmin = ceil(KNULL - 0.5 + 12 * (logFMinus[f] - logFK[fk]));
            kmax = floor(KNULL + 0.5 + 12 * (logFPlus[f] - logFK[fk]));

            float impuls = 0;

            if (kmin > 0 && kmin < numberOfKs && kmax > 0 && kmax < numberOfKs) {

                //calculates inner sum with lamdaK
                impuls = 0;
                float fplus = (float) (f + 0.5) / fk;
                float fminus = (float) (f - 0.5) / fk;

                for (int k = kmin; k <= kmax; k++) {

                    float oneDivided = (float) 1 / 24;
                    float minusOneDivided = (float) - 1 / 24;
                    float a = powf(2, oneDivided);
                    float b = powf(2, minusOneDivided);
                    float lamdaKTimesLamdaPlus = lamdaKForK(k) * a;
                    float lamdaKTimesLamdaMinus = lamdaKForK(k) * b;

                    impuls += component.impulsDistribution[k][t]*(min(lamdaKTimesLamdaPlus, fplus) - max(lamdaKTimesLamdaMinus, fminus));
                }

                //putting together the product and add to sum
                sum += spectogram[f][t] / (currentApprox2[f][t] + epsilon) * component.probabilityComponent*impuls;
            }
            //        }
        }
    }

    //returns product of current component Probability and sum
    return component.kernelDistribution[fk] * sum;

}


/***************************************************************************
 * Updates impulse distribution
 ***************************************************************************/

void NoteSeparator::updatesImpulseDistributionOfAllComponents(MskComponent updatedComponents[], float **currentApprox2, float **newImpulsDistribution2, int numberOfFixedPointIterations) {


    float sumOfImpulseDistributionUpdates = 0;

    for (int i = 0; i < numberOfComponents; i++) {

        //sets new impulsDistribution initially to the same values as the component iD
        for (int f = 0; f < numberOfKs; f++) {
            for (int t = 0; t < numberOfSamples; t++) {
                newImpulsDistribution2[f][t] = components[i].impulsDistribution[f][t];
            }
        }

        //provisorische Abbruchbedingung
        for (int n = 0; n < numberOfFixedPointIterations; n++) {

            sumOfImpulseDistributionUpdates = 0;

            //Calculates Update for every time-frequency bin and sums up all updates
            for (int kL = 1; kL < numberOfKs; kL++) {
                for (int t = 0; t < numberOfSamples; t++) {

                    //newImpulsDistribution is a Zwischenspeicher
                    newImpulsDistribution2[kL][t] = updateImpulseDistributionOfComponent(components[i], kL, t, currentApprox2, newImpulsDistribution2);
                    sumOfImpulseDistributionUpdates += newImpulsDistribution2[kL][t];
                }
            }

            //Updates impulse distribution of component by the normalized value
            for (int kL = 1; kL < numberOfKs; kL++) {

                float oneDivided = (float) 1 / 24;
                float minusOneDivided = (float) - 1 / 24;
                float a = powf(2, oneDivided);
                float b = powf(2, minusOneDivided);
                float lamdaKTimesLamdaPlus = lamdaKForK(kL) * a;
                float lamdaKTimesLamdaMinus = lamdaKForK(kL) * b;
                float deltaKl = lamdaKTimesLamdaPlus - lamdaKTimesLamdaMinus;


                for (int t = 0; t < numberOfSamples; t++) {

                    newImpulsDistribution2[kL][t] = newImpulsDistribution2[kL][t] / (deltaKl * sumOfImpulseDistributionUpdates + epsilon);
                }
            }
        }
        //Updates impulse distribution of component by the normalized value
        for (int kL = 1; kL < numberOfKs; kL++) {
            for (int t = 0; t < numberOfSamples; t++) {

                updatedComponents[i].impulsDistribution[kL][t] = newImpulsDistribution2[kL][t];
            }
        }

    }
}

//Updaterule 3; takes a component, a multipliCation number and the time

float NoteSeparator::updateImpulseDistributionOfComponent(MskComponent &component, int kL, int tGivenZ, float **currentApprox2, float **newImpulsDistribution2) {

    int kmin;
    int kmax;
    //  float innerSum;
    //int kmin2;
    //int kmax2;

    float sum = 0;
    float outerSum = 0;
    //calculates inner sum
    for (int fk = 1; fk < numberOfKernelFrequencies; fk++) {

        //outerSum = 0;

        //calculates first f for non negative k
        float firstF = pow(2, (2,5-KNULL) / 12 + logFK[fk]) + 0.5;
        float lastF = pow(2, (numberOfKs - KNULL - 0.5) / 12 + logFK[fk]) - 0.5;

        int firstFRound = ceil(firstF); //or ceil???
        int lastFRound = floor(lastF);

        if (firstFRound >= 1 && firstFRound <= lastFRound && lastFRound < numberOfFrequencies) {

            for (int f = firstFRound; f <= lastFRound; f++) {


                //calculates discretisation of lamda
                //kmin = ceil(KNULL - 0.5 + 12 * log2f((f - 0.5) / fk));
                //kmax = floor(KNULL + 0.5 + 12 * log2f((f + 0.5) / fk));

                kmin = ceil(KNULL - 0.5 + 12 * (logFMinus[f] - logFK[fk]));
                kmax = floor(KNULL + 0.5 + 12 * (logFPlus[f] - logFK[fk]));

                //if (kmin > 0 && kmax > 0 && kmax < numberOfKs && kmin < numberOfKs) {

                //variables
                float integralNumerator = 0;
                float integralDenominator = 0;
                float fplus = (float) (f + 0.5) / fk;
                float fminus = (float) (f - 0.5) / fk;
                float lamdaK;

                //indicatorfunction done by if-statement
                if (kmin <= kL && kL <= kmax) {

                    //calculates integral in numerator and denominator
                    for (int k = kmin; k <= kmax; k++) {

                        float oneDivided = (float) 1 / 24;
                        float minusOneDivided = (float) - 1 / 24;
                        float a = powf(2, oneDivided);
                        float b = powf(2, minusOneDivided);
                        float lamdaKTimesLamdaPlus = lamdaKForK(k) * a;
                        float lamdaKTimesLamdaMinus = lamdaKForK(k) * b;

                        integralNumerator += component.impulsDistribution[k][tGivenZ]*(min(lamdaKTimesLamdaPlus, fplus) - max(lamdaKTimesLamdaMinus, fminus));
                        integralDenominator += newImpulsDistribution2[k][tGivenZ]*(min(lamdaKTimesLamdaPlus, fplus) - max(lamdaKTimesLamdaMinus, fminus));
                        lamdaK = (min(lamdaKTimesLamdaPlus, fplus) - max(lamdaKTimesLamdaMinus, fminus));
                    }

                    sum +=
                            ((spectogram[f][tGivenZ] * integralNumerator * lamdaK) /
                            ((currentApprox2[f][tGivenZ] * integralDenominator) + epsilon));

                }

            }
        }
        

        outerSum += sum * component.kernelDistribution[fk];
        sum = 0;


    }
    //returns whole expression
    return newImpulsDistribution2[kL][tGivenZ] * component.probabilityComponent*outerSum;
}

void NoteSeparator::updatesCurrentApproximation(float **currentApprox2) {

   int kmin;
    int kmax;
    float innerSum =0;

    for (int f = 1; f < numberOfFrequencies; f++) {
        for (int t = 0; t < numberOfSamples; t++) {

            for (int z = 0; z < numberOfComponents; z++) {
                //first sum
                for (int fk = 1; fk < numberOfKernelFrequencies; fk++) {

                    //calculates the kmin and kmax for the approximation of P
                    kmin = ceil(KNULL - 0.5 + 12 * (logFMinus[f] - logFK[fk]));
                    kmax = floor(KNULL + 0.5 + 12 * (logFPlus[f] - logFK[fk]));


                    float fplus = (f + 0.5) / fk;
                    float fminus = (f - 0.5) / fk;

                    if (kmin > 0 && kmin < numberOfKs && kmax > 0 && kmax < numberOfKs) {

                        //inner sum
                        for (int k = kmin; k <= kmax; k++) {
                            float oneDivided = (float) 1 / 24;
                            float minusOneDivided = (float) - 1 / 24;
                            float a = powf(2, oneDivided);
                            float b = powf(2, minusOneDivided);
                            float lamdaKTimesLamdaPlus = lamdaKForK(k) * a;
                            float lamdaKTimesLamdaMinus = lamdaKForK(k) * b;
                            float minimum = (min(lamdaKTimesLamdaPlus, fplus) - max(lamdaKTimesLamdaMinus, fminus));

                            if (minimum > 0) {


                                innerSum += components[z].impulsDistribution[k][t] * minimum;
                            }
                        }
                    }

                    currentApprox2[f][t] += (components[z].probabilityComponent * components[z].kernelDistribution[fk]) * innerSum;



                    innerSum = 0;
                }
            }
        }
    }
}


void NoteSeparator::printApproximationToFileAndBmp(float** currentApprox2) {

    //find highest
    float highestValue = 0;
    for (int i = 0; i < numberOfFrequencies; i++) {
        for (int k = 0; k < numberOfSamples; k++) {
            if (currentApprox2[i][k] > highestValue)
                highestValue = currentApprox2[i][k];
        }
    }

    float multiplicationFactor = 250 / highestValue;
    BMP newBMP;
    newBMP.SetSize(numberOfFrequencies, numberOfSamples);
    newBMP.SetBitDepth(8);
    for (int i = 0; i < numberOfFrequencies; i++) {
        for (int k = 0; k < numberOfSamples; k++) {

            if (currentApprox2[i][k] * multiplicationFactor < 256) {
                newBMP(i, k)->Blue = int(currentApprox2[i][k] * multiplicationFactor);
                newBMP(i, k)->Alpha = int(currentApprox2[i][k] * multiplicationFactor);
                newBMP(i, k)->Green = int(currentApprox2[i][k] * multiplicationFactor);
                newBMP(i, k)->Red = int(currentApprox2[i][k] * multiplicationFactor);
            } else {
                newBMP(i, k)->Blue = 250;
                newBMP(i, k)->Alpha = 250;
                newBMP(i, k)->Green = 250;
                newBMP(i, k)->Red = 250;
            }

            //cout << impulsDistribution[i][k] << endl;
        }
    }
    newBMP.WriteToFile("currentApproximation.bmp");



    ///////////////

    ofstream myfile;
    myfile.open("CurrentApproximationTxt.txt");

    for (int r = 0; r < numberOfFrequencies; r++) {
        for (int c = 0; c < numberOfSamples; c++) {
            myfile << currentApprox2[r][c] << " ";
        }
        myfile << endl;
    }

    myfile.close();
}

/*int NoteSeparator::Ceil12TimesLog2(double x, bool plusMinus) {
    int exp;
    double mantissa = std::frexp(x, &exp) * 2;
    int idx = indexes[int((mantissa - 1.0) * 17)];
    if (plusMinus)
        return 12 * (exp - 1) + (mantissa <= roots[idx] ? idx : idx + 1);
    else
        return 12 * (exp - 1) + (mantissa < roots[idx] ? idx - 1 : idx);
}

/*double NoteSeparator::roots = {
    0x1.0000000000000p+0,
    0x1.0f38f92d97963p+0,
    0x1.1f59ac3c7d6c0p+0,
    0x1.306fe0a31b715p+0,
    0x1.428a2f98d728bp+0,
    0x1.55b8108f0ec5ep+0,
    0x1.6a09e667f3bccp+0,
    0x1.7f910d768cfb0p+0,
    0x1.965fea53d6e3dp+0,
    0x1.ae89f995ad3adp+0,
    0x1.c823e074ec129p+0,
    0x1.e3437e7101344p+0
};

int NoteSeparator::indexes = {0, 1, 2, 3, 4, 5, 5, 6, 7, 8, 8, 9, 9, 10, 10, 11, 11};*/














