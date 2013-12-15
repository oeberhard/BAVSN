/* 
 * File:   Component.cpp
 * Author: Boom
 * 
 * Created on 6. November 2012, 16:53
 */

#include "MskComponent.h"

MskComponent::MskComponent() {

}

MskComponent::MskComponent(int new_numberOfKs, int new_numberOfKernelFrequencies, int new_numberOfSamples) {

    numberOfKs = new_numberOfKs;
    numberOfKernelFrequencies = new_numberOfKernelFrequencies;
    numberOfSamples = new_numberOfSamples;
    probabilityComponent = 0;
    kernelDistribution = new float[new_numberOfKernelFrequencies];
    impulsDistribution = new float*[new_numberOfKs];
    for (int i = 0; i < new_numberOfKs; ++i)
        impulsDistribution[i] = new float[new_numberOfSamples];
}

MskComponent::MskComponent(const char* fileName) {

    ifstream file;
    file.open(fileName, ios::in);

    std::string buffer, tokenBuffer;

    getline(file, buffer);

    istringstream bufferStream(buffer);

    bufferStream >> numberOfKs;
    bufferStream >> numberOfSamples;
    bufferStream >> numberOfKernelFrequencies;
    /*
    getline(buffer, tokenBuffer, ' ');
    numberOfKs = atoi(tokenBuffer);

    getline(buffer, tokenBuffer, ' ');
    numberOfSamples = atoi(tokenBuffer);

    getline(buffer, tokenBuffer, ' ');
    numberOfKernelFrequencies = atoi(tokenBuffer);
     */
    probabilityComponent = 1;
    kernelDistribution = new float[numberOfKernelFrequencies];
    impulsDistribution = new float*[numberOfKs];

    for (int i = 0; i < numberOfKs; ++i)
        impulsDistribution[i] = new float[numberOfSamples];

    for (int i = 0; i < numberOfKs; i++) {

        getline(file, buffer);
        istringstream bufferStream(buffer);
        
        for (int k = 0; k < numberOfSamples; k++) {
            float token;
            bufferStream >> token;
            impulsDistribution[i][k] = token;
        }
    }

    for (int i = 0; i < numberOfKernelFrequencies; i++) {
        getline(file, tokenBuffer);
        const char* hallo = tokenBuffer.c_str();
        kernelDistribution[i] = atof(hallo);
    }

	    getline(file, buffer);
        istringstream bufferStream2(buffer);
		bufferStream2 >> sizeOfInput;
		bufferStream2 >> inputSamplerate;

		timeDomainInput = new double[sizeOfInput];

		getline(file, buffer);
		istringstream bufferStream3(buffer);

		for (int k = 0; k < sizeOfInput; k++) {
			float token;
			bufferStream3 >> token;
			timeDomainInput[k] = token;
		}




    file.close();
}

MskComponent::MskComponent(const MskComponent& orig) {
}

MskComponent::~MskComponent() {
}

void MskComponent::initializeComponent() {
    probabilityComponent = 1;

    float sumOfInits = 0;
    float randomNumber;

    for (int i = 0; i < numberOfKernelFrequencies; i++) {
        randomNumber = rand() % 100;
        kernelDistribution[i] = randomNumber;
        sumOfInits += randomNumber;
    }

    for (int i = 0; i < numberOfKernelFrequencies; i++) {
        kernelDistribution[i] = kernelDistribution[i] / sumOfInits;
    }

    sumOfInits = 0;

    for (int r = 0; r < numberOfKs; r++) {
        for (int c = 0; c < numberOfSamples; c++) {
            randomNumber = rand() % 100;
            impulsDistribution[r][c] = randomNumber;
            sumOfInits += randomNumber;
        }
    }
    for (int r = 0; r < numberOfKs; r++) {
        for (int c = 0; c < numberOfSamples; c++) {
            impulsDistribution[r][c] = impulsDistribution[r][c] / sumOfInits;
        }
    }
}

void MskComponent::printImpulseDistributionToFile() {

    //find highest
    float highestValue = 0;
    for (int i = 0; i < numberOfKs; i++) {
        for (int k = 0; k < numberOfSamples; k++) {
            if (impulsDistribution[i][k] > highestValue)
                highestValue = impulsDistribution[i][k];
        }
    }

    float multiplicationFactor = 250 / highestValue;
    BMP newBMP;
    newBMP.SetSize(numberOfKs, numberOfSamples);
    newBMP.SetBitDepth(8);
    for (int i = 0; i < numberOfKs; i++) {
        for (int k = 0; k < numberOfSamples; k++) {

            if (impulsDistribution[i][k] * multiplicationFactor < 256) {
                newBMP(i, k)->Blue = impulsDistribution[i][k] * multiplicationFactor;
                newBMP(i, k)->Alpha = impulsDistribution[i][k] * multiplicationFactor;
                newBMP(i, k)->Green = impulsDistribution[i][k] * multiplicationFactor;
                newBMP(i, k)->Red = impulsDistribution[i][k] * multiplicationFactor;
            } else {
                newBMP(i, k)->Blue = 250;
                newBMP(i, k)->Alpha = 250;
                newBMP(i, k)->Green = 250;
                newBMP(i, k)->Red = 250;
            }

            //cout << impulsDistribution[i][k] << endl;
        }
    }
    newBMP.WriteToFile("ImpulseDistribution.bmp");
}

void MskComponent::writeComponentToFile(double* timeDomainInput, int timeDomainInputSize, int samplerate) {

    ofstream myfile;

	int _rand = rand() % 100;
	stringstream filename;
	filename << "Component" << _rand << ".txt";
	myfile.open(filename.str());

    myfile << numberOfKs << " " << numberOfSamples << " " << numberOfKernelFrequencies << endl;

    for (int r = 0; r < numberOfKs; r++) {
        for (int c = 0; c < numberOfSamples; c++) {
            myfile << impulsDistribution[r][c] << " ";
        }
        myfile << endl;
    }

    for (int i = 0; i < numberOfKernelFrequencies; i++) {
        myfile << kernelDistribution[i];
        myfile << endl;
    }


	myfile << timeDomainInputSize << " " << samplerate << endl;

	for(int i = 0; i < timeDomainInputSize; i++)
		myfile << timeDomainInput[i] << " ";

    myfile.close();


}