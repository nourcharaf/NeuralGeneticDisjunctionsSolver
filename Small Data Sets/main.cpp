//
//  main.cpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/16/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#include <iostream>
#include <sstream>

#include "GeneticAlgorithm.hpp"

void startGeneticAlgorithm();
void printArray(std::vector<double> array);

int main() {
    
    // Seed for Random Number Generator
    srand((int)time(NULL));
    
    // Genetic Algorithm
    
    startGeneticAlgorithm();
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    
    std::vector<double> bestWeights = {0.987153,0.987153,0.45136,0.987153,0.987153,0.987153,0.45136,0.45136,0.987153,0.987153,-0.0842198,0.987153,0.987153,-0.833931,0.45136};
    Chromosome *chromosome = new Chromosome();
    Network *network = GeneticAlgorithm::createNetwork();
    chromosome->network = network;
    network->setEdgeWeights(bestWeights);
    
    std::vector<double> inputValues;
    std::vector<double> outputs;
    
    inputValues = {1,1};
    
    chromosome->network->feedForward(inputValues);
    
    outputs = network->getOutputs();
    
    inputValues = {1,0};
    
    chromosome->network->feedForward(inputValues);
    
    outputs = network->getOutputs();
    
    inputValues = {0,1};
    
    chromosome->network->feedForward(inputValues);
    
    outputs = network->getOutputs();
    
    inputValues = {0,0};
    
    chromosome->network->feedForward(inputValues);
    
    outputs = network->getOutputs();
    
    return 0;
}

void startGeneticAlgorithm(){
    
    GeneticAlgorithm *geneticAlgorithm = new GeneticAlgorithm();
    
    geneticAlgorithm->createChromosomes();
    
    geneticAlgorithm->processGenerations();
    
    Chromosome *bestChromosome = geneticAlgorithm->getBestChromosome();
    
    Network *bestNetwork = bestChromosome->network;
    
    printArray(bestNetwork->getEdgeWeights());
}

void printArray(std::vector<double> array){
    
    for (unsigned i = 0; i < array.size(); ++i){
        
        std::ostringstream strs;
        strs << array[i];
        std::string str = strs.str();
        
        std::cout << str + ",";
    }
}
