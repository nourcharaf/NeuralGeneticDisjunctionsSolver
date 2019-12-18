//
//  HyperGeneticAlgorithm.hpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/18/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#ifndef HyperGeneticAlgorithm_hpp
#define HyperGeneticAlgorithm_hpp

#include <stdio.h>

#include "HyperChromosome.hpp"

class HyperGeneticAlgorithm{
    
public:
    
    HyperChromosome *bestHyperChromosome;
    
    HyperGeneticAlgorithm();
    void createHyperChromosomes();
    void processGenerations();
    void processGeneration(unsigned generationNumber);
    void calculatePopulationFitnessValuesAndProbabilities();
    double calculateChromosomeFitness(HyperChromosome *hyperChromosome);
    void sortPopulation();
    std::vector<HyperChromosome *> selectElitePopulation();
    std::vector<HyperChromosome *> selectSurvivingPopulation();
    unsigned shouldSelect(double probability);
    std::vector<HyperChromosome *> crossover(std::vector<HyperChromosome *> survivingChromosomes);
    template<typename Iter> Iter select_randomly(Iter start, Iter end);
    double randomNumber();
    void mutation(std::vector<HyperChromosome *> offspringChromosomes);
    void logResults(unsigned generationNumber, HyperChromosome *chromosome);
    void printArray(std::vector<double> array);
    
};

#endif /* HyperGeneticAlgorithm_hpp */
