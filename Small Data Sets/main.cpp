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

#include "DataManager.hpp"

void printArray(std::vector<double> array);

int main() {
    
    // Seed for Random Number Generator
    srand((int)time(NULL));
    
    // Genetic Algorithm

//    GeneticAlgorithm *geneticAlgorithm = new GeneticAlgorithm();
//    geneticAlgorithm->createChromosomes();
//    geneticAlgorithm->processGenerations();
//    Network *bestNetwork = geneticAlgorithm->getBestChromosome()->network;
//    printArray(bestNetwork->getEdgeWeights());
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    
    std::vector<double> bestWeights = {-851.126,-851.126,704.56,-851.126,-851.126,-851.126,542.599,-851.126,704.56,704.56,-851.126,704.56,-851.126,704.56,-877.021,260.225,-851.126,-851.126,-851.126,-851.126,-851.126,-851.126,-851.126,704.56};
    Chromosome *chromosome = new Chromosome();
    Network *network = GeneticAlgorithm::createNetwork();
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

void printArray(std::vector<double> array){
    
    for (unsigned i = 0; i < array.size(); ++i){
        
        std::ostringstream strs;
        strs << array[i];
        std::string str = strs.str();
        
        std::cout << str + ",";
    }
}
