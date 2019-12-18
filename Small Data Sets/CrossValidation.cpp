//
//  CrossValidation.cpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/18/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#include "CrossValidation.hpp"

// Instances
std::vector<std::vector<double>> originalInstances = DataManager::getInstances("xor_data.txt");

// Cross Validation Parameters
double crossValidationFolds = 10;

CrossValidation::CrossValidation(){
    
}

void CrossValidation::setRandomRandomRange(){
    randomRange = int(1 + (rand() % 20));
}

void CrossValidation::setRandomTransferFunction(){
    std::vector<Network::TransferFunction> transferFunctions = Network::getTransferFunctions();
    transferFunction = int(0 + rand() % transferFunctions.size());
}

void CrossValidation::setRandomNumberOfHiddenLayers(){
    numberOfHiddenLayers = int(1 + (rand() % 10));
}

void CrossValidation::setRandomNumberOfHiddenNodes(){
    numberOfHiddenNodes = int(1 + (rand() % 10));
}

void CrossValidation::kFoldCrossValidation(){
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Instances
    
    // Seed for Random Number Generator
    srand((int)time(NULL));
    
    // Randomize Order of Instances
    std::default_random_engine randomEngine;
    std::shuffle(std::begin(originalInstances), std::end(originalInstances), randomEngine);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    
    std::vector<GeneticAlgorithm *> geneticAlgorithms;
    
    unsigned instancesSize = (int)originalInstances.size();
    unsigned numberOfTestInstances = instancesSize / crossValidationFolds;
    
    for (unsigned i = 0; i < crossValidationFolds; ++i){
        
        // Fold Number
        printf("Fold Number: %d\n",i + 1);
        
        // Testing indecies
        unsigned startTestIndex = i * numberOfTestInstances;
        unsigned endTestIndex = startTestIndex + numberOfTestInstances - 1;
        
        // Training Instances
        std::vector<std::vector<double>> trainingInstances;
        
        for (unsigned j = 0; j < startTestIndex; ++j){
            trainingInstances.push_back(originalInstances[j]);
        }
        
        for (unsigned j = endTestIndex + 1; j < instancesSize; ++j){
            trainingInstances.push_back(originalInstances[j]);
        }
        
        // Testing Instances
        std::vector<std::vector<double>> testingInstances;
        
        for (unsigned j = startTestIndex; j <= endTestIndex; ++j){
            testingInstances.push_back(originalInstances[j]);
        }
        
        // Genetic Algorithm
        GeneticAlgorithm *geneticAlgorithm = new GeneticAlgorithm();
        geneticAlgorithms.push_back(geneticAlgorithm);
        
        // Set Hyper Paramters
        geneticAlgorithm->randomRange = randomRange;
        geneticAlgorithm->transferFunction = transferFunction;
        geneticAlgorithm->numberOfHiddenLayers = numberOfHiddenLayers;
        geneticAlgorithm->numberOfHiddenNodes = numberOfHiddenNodes;
        
        // Training with training instances
        trainWithInstances(geneticAlgorithm, trainingInstances);
        
        // Test with testing instances
        testWithInstances(geneticAlgorithm,testingInstances);
    }
    
    // Report Final Results
    reportFinalResults(geneticAlgorithms);
}

void CrossValidation::trainWithInstances(GeneticAlgorithm *geneticAlgorithm, std::vector<std::vector<double>> instances){
    
    geneticAlgorithm->instances = instances;
    geneticAlgorithm->createChromosomes();
    geneticAlgorithm->processGenerations();
}

void CrossValidation::testWithInstances(GeneticAlgorithm *geneticAlgorithm, std::vector<std::vector<double>> instances){
    
    unsigned truePositives = 0;
    unsigned trueNegatives = 0;
    unsigned falsePositives = 0;
    unsigned falseNegatives = 0;
    
    for (unsigned i = 0; i < instances.size(); ++i){
        
        // Get Network
        Network *network = geneticAlgorithm->bestChromosome->network;
        
        // Split Instance into Inputs and Targets
        std::vector<double> instance = instances[i];
        
        // Input Values
        std::vector<double> inputValues;
        for (unsigned i = 0; i < instance.size() - 1; ++i){
            inputValues.push_back(instance[i]);
        }
        
        // Target Values
        std::vector<double> targetValues;
        targetValues.push_back(instance.back());
        
        // Feed Forward
        network->feedForward(inputValues);
        
        // Get Output
        std::vector<double> outputs = network->getOutputs();
        
        // Error
        double error = abs(targetValues[0] - outputs[0]);
        
        // Accuracy
        double accuracy = 1 - error;
        
        // Update Contingency Matrix
        if (accuracy >= 0.5){
            if (outputs[0] == 1){
                truePositives += 1;
            }
            else{
                trueNegatives += 1;
            }
        }
        else{
            if (outputs[0] == 1){
                falseNegatives += 1;
            }
            else{
                falsePositives += 1;
            }
        }
    }
    
    // Set Contingency Matrix to Genetic Algorithm
    geneticAlgorithm->truePositives = truePositives;
    geneticAlgorithm->trueNegatives = trueNegatives;
    geneticAlgorithm->falsePositives = falsePositives;
    geneticAlgorithm->falseNegatives = falseNegatives;
}

void CrossValidation::reportFinalResults(std::vector<GeneticAlgorithm *> geneticAlgorithms){
    
    std::vector<double> accuracies;
    double sumOfAccuracies = 0;
    
    std::vector<double> fmeasures;
    double sumeOfFMeasures = 0;
    
    for (unsigned i = 0; i < geneticAlgorithms.size(); ++i){
        
        GeneticAlgorithm *geneticAlgorithm = geneticAlgorithms[i];
        
        double tP = geneticAlgorithm->truePositives;
        double tN = geneticAlgorithm->trueNegatives;
        double fP = geneticAlgorithm->falsePositives;
        double fN = geneticAlgorithm->falseNegatives;
        
        // Accuracy
        double accuracy = (tP + tN + fP + fN != 0) ? (tP + tN)/(tP + tN + fP + fN) : 0;
        
        accuracies.push_back(accuracy);
        
        sumOfAccuracies += accuracy;
        
        // F-Measure
        double positivePredictiveValue = (tP + fP != 0) ? tP/(tP + fP) : 0;
        
        double sensitivity = (tP + fN != 0) ? tP/(tP + fN) : 0;
        
        double fmeasure = (positivePredictiveValue + sensitivity != 0) ? (2 * positivePredictiveValue * sensitivity)/(positivePredictiveValue + sensitivity) : 0;
        
        fmeasures.push_back(fmeasure);
        
        sumeOfFMeasures += fmeasure;
    }
    
    // Print
    std::cout << "Accuracies: " << std::endl;
    printArray(accuracies);
    std::cout << std::endl;
    
    averageAccuracy = sumOfAccuracies/crossValidationFolds;
    std::cout << "Average Accuracy: " + std::to_string(averageAccuracy) << std::endl;
    
    std::cout << "F-Measures: " << std::endl;
    printArray(fmeasures);
    std::cout << std::endl;
    
    averageFMeasure = sumeOfFMeasures/crossValidationFolds;
    std::cout << "Max F-Measure: " + std::to_string(averageFMeasure) << std::endl;
}

void CrossValidation::printArray(std::vector<double> array){
    
    for (unsigned i = 0; i < array.size(); ++i){
        
        std::ostringstream strs;
        strs << array[i];
        std::string str = strs.str();
        
        std::cout << str + ",";
    }
}
