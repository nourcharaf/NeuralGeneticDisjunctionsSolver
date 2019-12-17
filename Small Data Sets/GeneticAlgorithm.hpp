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
    
    GeneticAlgorithm();
    void createChromosomes();
    static Network * createNetwork();
    void processGenerations();
    void processGeneration();
    void calculatePopulationFitnessValuesAndProbabilities();
    double calculateChromosomeFitness(Chromosome *chromosome);
    void sortPopulation();
    std::vector<Chromosome *> selectElitePopulation();
    std::vector<Chromosome *> selectSurvivingPopulation();
    unsigned shouldSelect(double probability);
    std::vector<Chromosome *> crossover(std::vector<Chromosome *> survivingChromosomes);
    double randomNumber();
    void mutation(std::vector<Chromosome *> offspringChromosomes);
    Chromosome * getBestChromosome();
    
};

#endif /* GeneticAlgorithm_hpp */
