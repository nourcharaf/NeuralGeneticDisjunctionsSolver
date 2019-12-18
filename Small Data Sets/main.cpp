//
//  main.cpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/16/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#include "HyperGeneticAlgorithm.hpp"

int main(){
    
    // Set Out Own Hyper Parameters
    CrossValidation *crossValidation = new CrossValidation();
    
    crossValidation->randomRange = 10;
    crossValidation->transferFunction = Network::hyperbolicTangent;
    crossValidation->numberOfHiddenLayers = 1;
    crossValidation->numberOfHiddenNodes = 4;
    
    crossValidation->kFoldCrossValidation();
    
    double averageAccuracy = crossValidation->averageAccuracy;
    double averageFMeasure = crossValidation->averageFMeasure;
    
    // Find Hyper Parameters Using Hyper Genetic Algorithm
    HyperGeneticAlgorithm *hyperGeneticAlgorithm = new HyperGeneticAlgorithm();
    hyperGeneticAlgorithm->createHyperChromosomes();
    hyperGeneticAlgorithm->processGenerations();
    
    CrossValidation *bestCrossValidation = hyperGeneticAlgorithm->bestHyperChromosome->crossValidation;
    double bestAverageAccuracy = bestCrossValidation->averageAccuracy;
    double bestFMeasure = bestCrossValidation->averageFMeasure;
}


