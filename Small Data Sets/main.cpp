//
//  main.cpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/16/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#include "HyperGeneticAlgorithm.hpp"

// Hyper Parameters
unsigned bestRandomRange = 10;
unsigned bestTransferFunction = Network::hyperbolicTangent;
unsigned bestNumberOfHiddenLayers = 1;
unsigned bestNumberOfHiddenNodes = 5;

int main(){
    
    // Process Instances
    // Seed for Random Number Generator
    srand((int)time(NULL));
    
    // Randomize Order of Instances
    std::default_random_engine randomEngine;
    std::shuffle(std::begin(seenInstances), std::end(seenInstances), randomEngine);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Note: This part can be commented out so that hyper parameters remain fixed
    /*
    // Find Hyper Parameters Using Hyper Genetic Algorithm
    HyperGeneticAlgorithm *hyperGeneticAlgorithm = new HyperGeneticAlgorithm();
    hyperGeneticAlgorithm->createHyperChromosomes();
    hyperGeneticAlgorithm->processGenerations();
    
    CrossValidation *bestCrossValidation = hyperGeneticAlgorithm->bestHyperChromosome->crossValidation;
    
    bestRandomRange = bestCrossValidation->randomRange;
    bestTransferFunction = bestCrossValidation->transferFunction;
    bestNumberOfHiddenLayers = bestCrossValidation->numberOfHiddenLayers;
    bestNumberOfHiddenNodes = bestCrossValidation->numberOfHiddenNodes;
    */
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Cross Validation with Hyper Parameters
    CrossValidation *crossValidation = new CrossValidation();
    
    crossValidation->randomRange = bestRandomRange;
    crossValidation->transferFunction = bestTransferFunction;
    crossValidation->numberOfHiddenLayers = bestNumberOfHiddenLayers;
    crossValidation->numberOfHiddenNodes = bestNumberOfHiddenNodes;
    
    crossValidation->kFoldCrossValidation(); // This also reports Average Test Accuracy and Average Test F-Measure
    
    // Test Unseen Data
    GeneticAlgorithm *highestTestAccuracyGeneticAlgorithm = crossValidation->highestTestAccuracyGeneticAlgorithm;
    crossValidation->testWithInstances(highestTestAccuracyGeneticAlgorithm, unseenInstances);
    
    // Report Average Unseen Test Accuracy and Average Unseen Test F-Measure
    double averageUnseenTestAccuracy = crossValidation->getAccuracy(highestTestAccuracyGeneticAlgorithm);
    double averageUnseenTestFMeasure = crossValidation->getFMeasure(highestTestAccuracyGeneticAlgorithm);
    
    std::cout << "Average Unseen Test Accuracy: " + std::to_string(averageUnseenTestAccuracy) << std::endl;
    std::cout << "Average Unseen Test F-Measure: " + std::to_string(averageUnseenTestFMeasure) << std::endl;
    
    std::cout << "Unseen True Positives: " + std::to_string(highestTestAccuracyGeneticAlgorithm->truePositives) << std::endl;
    std::cout << "Unseen True Negatives: " + std::to_string(highestTestAccuracyGeneticAlgorithm->trueNegatives) << std::endl;
    std::cout << "Unseen False Positives: " + std::to_string(highestTestAccuracyGeneticAlgorithm->falsePositives) << std::endl;
    std::cout << "Unseen False Negatives: " + std::to_string(highestTestAccuracyGeneticAlgorithm->falseNegatives) << std::endl;
}

