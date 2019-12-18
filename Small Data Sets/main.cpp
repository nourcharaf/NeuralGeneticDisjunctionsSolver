//
//  main.cpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/16/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#include "GeneticAlgorithm.hpp"

#include "DataManager.hpp"

void printArray(std::vector<double> array);

int main() {
    
    // Seed for Random Number Generator
    srand((int)time(NULL));
    
    // Genetic Algorithm
    
    GeneticAlgorithm *geneticAlgorithm = new GeneticAlgorithm();
    
    geneticAlgorithm->createChromosomes();
    geneticAlgorithm->processGenerations();
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    
    std::vector<double> bestWeights = {6.78058,-7.84628,-4.94071,-2.49621,6.78058,-2.49621,-4.94071,-7.84628,-7.35246,5.97837,6.78058,6.78058,-1.60836,6.78058,-4.94071,6.78058,6.78058,-4.94071,6.78058,-4.94071,6.78058,-2.49621,6.78058,-4.94071};
    Chromosome *chromosome = new Chromosome();
    Network *network = geneticAlgorithm->createNetwork();
    chromosome->network = network;
    network->setEdgeWeights(bestWeights);
    
    std::vector<double> outputs;
    
    chromosome->network->feedForward({1,1,1}); // 1
    outputs = network->getOutputs();
    
    chromosome->network->feedForward({1,1,0}); // 0
    outputs = network->getOutputs();
    
    chromosome->network->feedForward({1,0,1}); // 0
    outputs = network->getOutputs();
    
    chromosome->network->feedForward({1,0,0}); // 1
    outputs = network->getOutputs();
    
    chromosome->network->feedForward({0,1,1}); // 0
    outputs = network->getOutputs();
    
    chromosome->network->feedForward({0,1,0}); // 1
    outputs = network->getOutputs();
    
    chromosome->network->feedForward({0,0,1}); // 1
    outputs = network->getOutputs();
    
    chromosome->network->feedForward({0,0,0}); // 0
    outputs = network->getOutputs();
    
    return 0;
}


