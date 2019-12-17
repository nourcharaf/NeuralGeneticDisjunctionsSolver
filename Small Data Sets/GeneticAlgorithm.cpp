//
//  GeneticAlgorithm.cpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/16/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#include "GeneticAlgorithm.hpp"

#include <string>
#include <random>
#include <iterator>

#include "DataManager.hpp"

// Parameters

std::vector<std::vector<double>> instances = DataManager::getInstances("xor_data.txt");
unsigned numberOfOutputs = 1;
unsigned numberOfInputs = (unsigned)instances[0].size() - numberOfOutputs;

Network::TransferFunction transferFunction = Network::hyperbolicTangent;
std::vector<unsigned> topology = {numberOfInputs,2,numberOfOutputs};

std::vector<Chromosome *> chromosomes;
unsigned populationSize = 1000;
unsigned numberOfGenerations = 1000;
double elitePercentage = 0.05;
unsigned elitePopulationSize = unsigned(elitePercentage * populationSize);
unsigned offspringPopulationSize = populationSize - elitePopulationSize;
double survivingPercentage = 0.1;
unsigned survivingPopulationSize = unsigned(survivingPercentage * populationSize);
double crossoverProbability = 0.5;
double mutationProbability = 0.01;

// Other

std::random_device randomDevice;
std::mt19937 mt19937(randomDevice());
unsigned uniformIntDistributionSize = 1000;

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    
    std::uniform_int_distribution<int> uniformIntDistribution(0, unsigned(std::distance(start, end)) - 1);
    
    std::advance(start, uniformIntDistribution(mt19937));
    
    return start;
}

GeneticAlgorithm::GeneticAlgorithm(){
    
}

void GeneticAlgorithm::createChromosomes(){
    
    for (unsigned i = 0; i < populationSize; ++i){
        
        Chromosome *chromosome = new Chromosome();
        chromosomes.push_back(chromosome);
        
        chromosome->network = createNetwork();
        chromosome->network->setRandomEdgeWeights();
    }
}

Network * GeneticAlgorithm::createNetwork(){
    
    Network *network = new Network();
    
    network->setTransferFunction(transferFunction);
    network->setTopology(topology);
    network->createLayers();
    
    return network;
}

void GeneticAlgorithm::processGenerations(){
    
    for (unsigned i = 0; i < numberOfGenerations; ++i){
        processGeneration();
    }
}

void GeneticAlgorithm::processGeneration(){
    
    // Fitness: Calculate Population Fitness Values and Probabilities
    calculatePopulationFitnessValuesAndProbabilities();
    
    // Sort: Sort Population based on Fitness Value
    sortPopulation();
    
    // Elitism: Select Elite Population
    std::vector<Chromosome *> eliteChromosomes = selectElitePopulation();
    
    // Selection: Select Surviving Population: Roulette Wheel Selection
    std::vector<Chromosome *> survivingChromosomes = selectSurvivingPopulation();
    
    // Crossover: Crossover Surviving Population using Uniform Crossover to Generate Offspring Population
    std::vector<Chromosome *>offspringChromosomes = crossover(survivingChromosomes);
    
    // Mutatation: Mutate Offspring Chromosomes
    mutation(offspringChromosomes);
    
    // Replace: Replace existing population with elite population and offspring population
    chromosomes.clear();
    chromosomes.insert(chromosomes.end(), eliteChromosomes.begin(), eliteChromosomes.end());
    chromosomes.insert(chromosomes.end(), offspringChromosomes.begin(), offspringChromosomes.end());
}

void GeneticAlgorithm:: calculatePopulationFitnessValuesAndProbabilities(){
    
    // Fitness Values
    double sumOfFitnessValues = 0;
    for (unsigned i = 0; i < chromosomes.size(); ++i){
        Chromosome *chromosome = chromosomes[i];
        double fitnessValue = calculateChromosomeFitness(chromosome);
        sumOfFitnessValues += fitnessValue;
    }
    
    // Probabilities
    for (unsigned i = 0; i < chromosomes.size(); ++i){
        Chromosome *chromosome = chromosomes[i];
        double fitnessValue = chromosome->fitnessValue;
        chromosome->setProbability(fitnessValue/sumOfFitnessValues);
    }
}

double GeneticAlgorithm::calculateChromosomeFitness(Chromosome *chromosome){
    
    double sumOfErrors = 0;
    
    for (unsigned i = 0; i < instances.size(); ++i){
        
        // Get Network
        Network *network = chromosome->network;
        
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
        
        // Add All Errors
        sumOfErrors += error;
    }
    
    // Set Fitness: Max fitness value is the total number of instances
    double maxFitnessValue = instances.size();
    chromosome->setFitnessValue(maxFitnessValue - sumOfErrors);
    
    return chromosome->fitnessValue;
}

void GeneticAlgorithm::sortPopulation(){
    std::sort(chromosomes.begin(), chromosomes.end(), [](Chromosome *a, Chromosome *b) {
        return a->fitnessValue > b->fitnessValue;
    });
}

std::vector<Chromosome *> GeneticAlgorithm::selectElitePopulation(){
    
    std::vector<Chromosome *> eliteChromosomes;
    
    for (unsigned i = 0; i < elitePopulationSize; ++i){
        Chromosome *chromosome = chromosomes[i];
        eliteChromosomes.push_back(chromosome);
    }
    
    return eliteChromosomes;
}

std::vector<Chromosome *> GeneticAlgorithm::selectSurvivingPopulation(){
    
    std::vector<Chromosome *> survivingChromosomes;
    
    while (1){
        
        bool didReachDesiredSize = false;
        
        for (unsigned i = elitePopulationSize + 1; i < populationSize; ++i){
            
            Chromosome *chromosome = chromosomes[i];
            
            // Probability
            if (shouldSelect(chromosome->probability)){
                
                // Add Surviving Chromosome
                survivingChromosomes.push_back(chromosome);
                
                // Check if reached desired Surviving Population Size
                if (survivingChromosomes.size() >= survivingPopulationSize){
                    didReachDesiredSize = true;
                    break;
                }
            }
        }
        
        if (didReachDesiredSize){
            break;
        }
    }
    
    return survivingChromosomes;
}

unsigned GeneticAlgorithm::shouldSelect(double probability){
    
    std::uniform_int_distribution<int> uniformIntDistribution(1, uniformIntDistributionSize);
    
    unsigned value = uniformIntDistribution(mt19937);
    
    unsigned threshold = probability * uniformIntDistributionSize;
    
    return threshold >= value;
}

std::vector<Chromosome *> GeneticAlgorithm::crossover(std::vector<Chromosome *> survivingChromosomes){
    
    std::vector<Chromosome *> offspringChromosomes;
    
    while (1) {
        
        Chromosome *parent1 = *select_randomly(survivingChromosomes.begin(), survivingChromosomes.end());
        std::vector<double> weights1 = parent1->network->getEdgeWeights();
        std::vector<double> newWeights1;
        
        Chromosome *parent2 = *select_randomly(survivingChromosomes.begin(), survivingChromosomes.end());
        std::vector<double> weights2 = parent2->network->getEdgeWeights();
        std::vector<double> newWeights2;
        
        for (unsigned i = 0; i < weights1.size(); ++i){
            
            double weight1 = weights1[i];
            
            for (unsigned j = 0; j < weights2.size(); ++j){
                
                double weight2 = weights2[j];
                
                if (randomNumber() <= crossoverProbability){
                    newWeights1.push_back(weight1);
                    newWeights2.push_back(weight2);
                }
                else{
                    newWeights1.push_back(weight2);
                    newWeights2.push_back(weight1);
                }
            }
        }
        
        Chromosome *offspring1 = new Chromosome();
        offspring1->network = createNetwork();
        offspring1->network->setEdgeWeights(newWeights1);
        
        if (offspringChromosomes.size() != offspringPopulationSize){
            offspringChromosomes.push_back(offspring1);
        }
        else{
            break;
        }
        
        Chromosome *offspring2 = new Chromosome();
        offspring2->network = createNetwork();
        offspring2->network->setEdgeWeights(newWeights2);
        
        if (offspringChromosomes.size() != offspringPopulationSize){
            offspringChromosomes.push_back(offspring2);
        }
        else{
            break;
        }
    }
    
    return offspringChromosomes;
}

double GeneticAlgorithm::randomNumber(){
    return rand()/double(RAND_MAX);
}

void GeneticAlgorithm::mutation(std::vector<Chromosome *> offspringChromosomes){
    
    for (unsigned i = 0; i < offspringChromosomes.size(); ++i){
        
        Chromosome *chromosome = offspringChromosomes[i];
        std::vector<double> weights = chromosome->network->getEdgeWeights();
        bool didMutate = false;
        
        for (unsigned j = 0; j < weights.size(); ++j){
            
            if (shouldSelect(mutationProbability)){
                weights[j] = Network::randomWeight();
                didMutate = true;
            }
        }
        
        if (didMutate){
            chromosome->network->setEdgeWeights(weights);
        }
    }
}

Chromosome * GeneticAlgorithm::getBestChromosome(){
    return chromosomes[0];
}
