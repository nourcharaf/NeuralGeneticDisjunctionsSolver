//
//  HyperGeneticAlgorithm.cpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/18/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#include "HyperGeneticAlgorithm.hpp"

// Hyper Genetic Algorithm Parameters
unsigned hyperPopulationSize = 1000;
unsigned hyperNumberOfGenerations = 20;
double hyperDesiredAccuracy = 0.9;
double hyperElitePercentage = 0.1;
double hyperSurvivingPercentage = 0.1;
double hyperCrossoverProbability = 0.5;
double hyperMaxMutationProbability = 1;
double hyperMutationDecrement = 0.02;
double hyperProbabilityIncrement = 0.01;
unsigned hyperUniformIntDistributionSize = 1000;

////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<HyperChromosome *> hyperChromosomes;
std::vector<HyperChromosome *> bestHyperChromosomes;
unsigned hyperElitePopulationSize = unsigned(hyperElitePercentage * hyperPopulationSize);
unsigned hyperOffspringPopulationSize = hyperPopulationSize - hyperElitePercentage;
unsigned hyperSurvivingPopulationSize = unsigned(hyperSurvivingPercentage * hyperPopulationSize);
double hyperMutationProbability = hyperMaxMutationProbability;
std::random_device hyperRandomDevice;
std::mt19937 hyperMt19937(hyperRandomDevice());

HyperGeneticAlgorithm::HyperGeneticAlgorithm(){
    
}

void HyperGeneticAlgorithm::createHyperChromosomes(){
    
    for (unsigned i = 0; i < hyperPopulationSize; ++i){
        
        HyperChromosome *hyperChromosome = new HyperChromosome();
        hyperChromosomes.push_back(hyperChromosome);
        
        CrossValidation *crossValidation = new CrossValidation();
        hyperChromosome->crossValidation = crossValidation;
        
        crossValidation->setRandomRandomRange();
        crossValidation->setRandomTransferFunction();
        crossValidation->setRandomNumberOfHiddenLayers();
        crossValidation->setRandomNumberOfHiddenNodes();
    }
}

void HyperGeneticAlgorithm::processGenerations(){
    
    for (unsigned i = 0; i < hyperNumberOfGenerations; ++i){
        
        // Process Generation
        processGeneration(i);
        
        // Update Mutation Probability
        hyperMutationProbability -= hyperMutationDecrement;
        if (hyperMutationProbability < 0){
            hyperMutationProbability = hyperMaxMutationProbability;
        }
        
        // Check for convergance
        
        
        // Check if met desired Accuracy
        if (bestHyperChromosome->fitnessValue >= hyperDesiredAccuracy){
            break;
        }
    }
}

void HyperGeneticAlgorithm::processGeneration(unsigned generationNumber){
    
    // Fitness: Calculate Population Fitness Values and Probabilities
    calculatePopulationFitnessValuesAndProbabilities();
    
    // Sort: Sort Population based on Fitness Value
    sortPopulation();
    
    // Elitism: Select Elite Population
    std::vector<HyperChromosome *> eliteHyperChromosomes = selectElitePopulation();
    
    // Selection: Select Surviving Population: Roulette Wheel Selection
    std::vector<HyperChromosome *> survivingHyperChromosomes = selectSurvivingPopulation();
    
    // Crossover: Crossover Surviving Population using Uniform Crossover to Generate Offspring Population
    std::vector<HyperChromosome *>offspringHyperChromosomes = crossover(survivingHyperChromosomes);
    
    // Mutatation: Mutate Offspring Chromosomes
    mutation(offspringHyperChromosomes);
    
    // Replace: Replace existing population with elite population and offspring population
    hyperChromosomes.clear();
    hyperChromosomes.insert(hyperChromosomes.end(), eliteHyperChromosomes.begin(), eliteHyperChromosomes.end());
    hyperChromosomes.insert(hyperChromosomes.end(), offspringHyperChromosomes.begin(), offspringHyperChromosomes.end());
    
    // Keep track of Best Chromosome
    bestHyperChromosome = hyperChromosomes[0];
    bestHyperChromosomes.push_back(bestHyperChromosome);
    
    // Log Results of Best Chromosome
    logResults(generationNumber, bestHyperChromosome);
}

void HyperGeneticAlgorithm:: calculatePopulationFitnessValuesAndProbabilities(){
    
    // Fitness Values
    double sumOfFitnessValues = 0;
    for (unsigned i = 0; i < hyperChromosomes.size(); ++i){
        HyperChromosome *hyperChromosome = hyperChromosomes[i];
        double fitnessValue = calculateChromosomeFitness(hyperChromosome);
        sumOfFitnessValues += fitnessValue;
    }
    
    // Probabilities
    for (unsigned i = 0; i < hyperChromosomes.size(); ++i){
        HyperChromosome *hyperChromosome = hyperChromosomes[i];
        double fitnessValue = hyperChromosome->fitnessValue;
        double probability = fitnessValue/sumOfFitnessValues;
        hyperChromosome->probability = probability;
    }
}

double HyperGeneticAlgorithm::calculateChromosomeFitness(HyperChromosome *hyperChromosome){
    
    CrossValidation *crossValidation = hyperChromosome->crossValidation;
    crossValidation->kFoldCrossValidation();
    
    double averageAccuracy = crossValidation->averageAccuracy;
    
    hyperChromosome->fitnessValue = averageAccuracy;
    
    return hyperChromosome->fitnessValue;
}

void HyperGeneticAlgorithm::sortPopulation(){
    std::sort(hyperChromosomes.begin(), hyperChromosomes.end(), [](HyperChromosome *a, HyperChromosome *b) {
        return a->fitnessValue > b->fitnessValue;
    });
}

std::vector<HyperChromosome *> HyperGeneticAlgorithm::selectElitePopulation(){
    
    std::vector<HyperChromosome *> eliteHyperChromosomes;
    
    for (unsigned i = 0; i < hyperElitePopulationSize; ++i){
        HyperChromosome *hyperChromosome = hyperChromosomes[i];
        eliteHyperChromosomes.push_back(hyperChromosome);
    }
    
    return eliteHyperChromosomes;
}

std::vector<HyperChromosome *> HyperGeneticAlgorithm::selectSurvivingPopulation(){
    
    std::vector<HyperChromosome *> survivingHyperChromosomes;
    
    while (1){
        
        bool didReachDesiredSize = false;
        
        for (unsigned i = hyperElitePopulationSize + 1; i < hyperPopulationSize; ++i){
            
            HyperChromosome *hyperChromosome = hyperChromosomes[i];
            
            // Probability
            if (shouldSelect(hyperChromosome->probability)){
                
                // Add Surviving Chromosome
                survivingHyperChromosomes.push_back(hyperChromosome);
                
                // Check if reached desired Surviving Population Size
                if (survivingHyperChromosomes.size() >= hyperSurvivingPopulationSize){
                    didReachDesiredSize = true;
                    break;
                }
            }
            else{
                // Increase chromosomes probability
                hyperChromosome->probability *= (1 + hyperProbabilityIncrement);
            }
        }
        
        if (didReachDesiredSize){
            break;
        }
    }
    
    return survivingHyperChromosomes;
}

unsigned HyperGeneticAlgorithm::shouldSelect(double probability){
    
    std::uniform_int_distribution<int> uniformIntDistribution(1, hyperUniformIntDistributionSize);
    
    unsigned value = uniformIntDistribution(hyperMt19937);
    
    unsigned threshold = probability * hyperUniformIntDistributionSize;
    
    return threshold >= value;
}

// This method is used to select randomly from array
template<typename Iter> Iter selectRandomly(Iter start, Iter end) {
    
    std::uniform_int_distribution<int> uniformIntDistribution(0, unsigned(std::distance(start, end)) - 1);
    
    std::advance(start, uniformIntDistribution(hyperMt19937));
    
    return start;
}

std::vector<HyperChromosome *> HyperGeneticAlgorithm::crossover(std::vector<HyperChromosome *> survivingHyperChromosomes){
    
    std::vector<HyperChromosome *> offspringHyperChromosomes;
    
    while (1) {
        
        HyperChromosome *parent1 = *selectRandomly(survivingHyperChromosomes.begin(), survivingHyperChromosomes.end());
        
        HyperChromosome *parent2 = *selectRandomly(survivingHyperChromosomes.begin(), survivingHyperChromosomes.end());
        
        HyperChromosome *offspring1 = new HyperChromosome();
        offspring1->crossValidation = new CrossValidation();
        
        HyperChromosome *offspring2 = new HyperChromosome();
        offspring2->crossValidation = new CrossValidation();
        
        if (randomNumber() <= hyperCrossoverProbability){
            offspring1->crossValidation->randomRange = parent1->crossValidation->randomRange;
            offspring2->crossValidation->randomRange = parent2->crossValidation->randomRange;
        }
        else{
            offspring1->crossValidation->randomRange = parent2->crossValidation->randomRange;
            offspring2->crossValidation->randomRange = parent1->crossValidation->randomRange;
        }
        
        if (randomNumber() <= hyperCrossoverProbability){
            offspring1->crossValidation->transferFunction = parent1->crossValidation->transferFunction;
            offspring2->crossValidation->transferFunction = parent2->crossValidation->transferFunction;
        }
        else{
            offspring1->crossValidation->transferFunction = parent2->crossValidation->transferFunction;
            offspring2->crossValidation->transferFunction = parent1->crossValidation->transferFunction;
        }
        
        if (randomNumber() <= hyperCrossoverProbability){
            offspring1->crossValidation->numberOfHiddenLayers = parent1->crossValidation->numberOfHiddenLayers;
            offspring2->crossValidation->numberOfHiddenLayers = parent2->crossValidation->numberOfHiddenLayers;
        }
        else{
            offspring1->crossValidation->numberOfHiddenLayers = parent2->crossValidation->numberOfHiddenLayers;
            offspring2->crossValidation->numberOfHiddenLayers = parent1->crossValidation->numberOfHiddenLayers;
        }
        
        if (randomNumber() <= hyperCrossoverProbability){
            offspring1->crossValidation->numberOfHiddenNodes = parent1->crossValidation->numberOfHiddenNodes;
            offspring2->crossValidation->numberOfHiddenNodes = parent2->crossValidation->numberOfHiddenNodes;
        }
        else{
            offspring1->crossValidation->numberOfHiddenNodes = parent2->crossValidation->numberOfHiddenNodes;
            offspring2->crossValidation->numberOfHiddenNodes = parent1->crossValidation->numberOfHiddenNodes;
        }
        
        if (offspringHyperChromosomes.size() != hyperOffspringPopulationSize){
            offspringHyperChromosomes.push_back(offspring1);
        }
        else{
            break;
        }
        
        if (offspringHyperChromosomes.size() != hyperOffspringPopulationSize){
            offspringHyperChromosomes.push_back(offspring2);
        }
        else{
            break;
        }
    }
    
    return offspringHyperChromosomes;
}

double HyperGeneticAlgorithm::randomNumber(){
    return rand()/double(RAND_MAX);
}

void HyperGeneticAlgorithm::mutation(std::vector<HyperChromosome *> offspringHyperChromosomes){
    
    for (unsigned i = 0; i < offspringHyperChromosomes.size(); ++i){
        
        HyperChromosome *hyperChromosome = offspringHyperChromosomes[i];
        CrossValidation *crossValidation = hyperChromosome->crossValidation;
        
        if (shouldSelect(hyperMutationProbability)){
            crossValidation->setRandomRandomRange();
        }
        
        if (shouldSelect(hyperMutationProbability)){
            crossValidation->setRandomTransferFunction();
        }
        
        if (shouldSelect(hyperMutationProbability)){
            crossValidation->setRandomNumberOfHiddenLayers();
        }
        
        if (shouldSelect(hyperMutationProbability)){
            crossValidation->setRandomNumberOfHiddenNodes();
        }
    }
}

void HyperGeneticAlgorithm::logResults(unsigned generationNumber, HyperChromosome *hyperChromosome){
    
    std::cout << "Generation Number: " + std::to_string(generationNumber) << std::endl;
    
    std::cout << "Fitness Value: " + std::to_string(hyperChromosome->fitnessValue) << std::endl;
    
    std::cout << "Mutation Probability: " + std::to_string(hyperMutationProbability) << std::endl;
    
    std::cout << "Average Accuracy: " + std::to_string(hyperChromosome->crossValidation->averageAccuracy) << std::endl;
}

void HyperGeneticAlgorithm::printArray(std::vector<double> array){
    
    for (unsigned i = 0; i < array.size(); ++i){
        
        std::ostringstream strs;
        strs << array[i];
        std::string str = strs.str();
        
        std::cout << str + ",";
    }
}

