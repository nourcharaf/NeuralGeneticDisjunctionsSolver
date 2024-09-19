//
//  GeneticAlgorithm.hpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/16/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#ifndef GeneticAlgorithm_hpp
#define GeneticAlgorithm_hpp

#include <stdio.h>

#include "Chromosome.hpp"

class GeneticAlgorithm{
    
public:
    
    // Stored Results: Accuracy of Best Chromosome (Neural Network)
    unsigned truePositives;
    unsigned trueNegatives;
    unsigned falsePositives;
    unsigned falseNegatives;
    unsigned testAccuracy;
    
    // Hyper Parameters for Neural Networks
    unsigned randomRange;
    unsigned transferFunction;
    unsigned numberOfHiddenLayers;
    unsigned numberOfHiddenNodes;
    
    std::vector<std::vector<double>> instances;
    Chromosome *bestChromosome;
    
    GeneticAlgorithm();
    void createChromosomes();
    Network * createNetwork();
    void processGenerations();
    void processGeneration(unsigned generationNumber);
    void calculatePopulationFitnessValuesAndProbabilities();
    double calculateChromosomeFitness(Chromosome *chromosome);
    void sortPopulation();
    std::vector<Chromosome *> selectElitePopulation();
    std::vector<Chromosome *> selectSurvivingPopulation();
    unsigned shouldSelect(double probability);
    std::vector<Chromosome *> crossover(std::vector<Chromosome *> survivingChromosomes);
    template<typename Iter> Iter select_randomly(Iter start, Iter end);
    double randomNumber();
    void mutation(std::vector<Chromosome *> offspringChromosomes);
    void logResults(unsigned generationNumber, Chromosome *chromosome);
    void printArray(std::vector<double> array);
    
};

#endif /* GeneticAlgorithm_hpp */
