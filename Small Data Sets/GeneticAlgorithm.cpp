//
//  GeneticAlgorithm.cpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/16/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#include "GeneticAlgorithm.hpp"

// Neural Network Parameters

// Random Range
double randomRange = 10;

// Transfer Function
Network::TransferFunction transferFunction = Network::hyperbolicTangent;

// Topology
unsigned numberOfInputs = 3;
unsigned numberOfOutputs = 1;
std::vector<unsigned> topology = {numberOfInputs,3,numberOfOutputs};

// Genetic Algorithm Parameters
unsigned populationSize = 1000;
unsigned numberOfGenerations = 500;
double desiredAccuracy = 0.9;
double elitePercentage = 0.1;
double survivingPercentage = 0.1;
double crossoverProbability = 0.5;
double maxMutationProbability = 1;
double mutationDecrement = 0.02;
double probabilityIncrement = 0.01;
unsigned uniformIntDistributionSize = 1000;

////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<Chromosome *> chromosomes;
std::vector<Chromosome *> bestChromosomes;
unsigned elitePopulationSize = unsigned(elitePercentage * populationSize);
unsigned offspringPopulationSize = populationSize - elitePopulationSize;
unsigned survivingPopulationSize = unsigned(survivingPercentage * populationSize);
double mutationProbability = maxMutationProbability;
std::random_device randomDevice;
std::mt19937 mt19937(randomDevice());

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
    
    network->randomRange = randomRange;
    network->transferFunction = transferFunction;
    network->topology = topology;
    network->createLayers();
    
    return network;
}

void GeneticAlgorithm::processGenerations(){
    
    for (unsigned i = 0; i < numberOfGenerations; ++i){
        
        // Process Generation
        processGeneration(i);
        
        // Update Mutation Probability
        mutationProbability -= mutationDecrement;
        if (mutationProbability < 0){
            mutationProbability = maxMutationProbability;
        }
        
        // Check for convergance
        
        
        // Check if met desired Accuracy
        if (bestChromosome->fitnessValue >= desiredAccuracy){
            break;
        }
    }
}

void GeneticAlgorithm::processGeneration(unsigned generationNumber){
    
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
    
    // Keep track of Best Chromosome
    bestChromosome = chromosomes[0];
    bestChromosomes.push_back(bestChromosome);
    
    // Log Results of Best Chromosome
    logResults(generationNumber, bestChromosome);
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
        double probability = fitnessValue/sumOfFitnessValues;
        chromosome->probability = probability;
    }
}

double GeneticAlgorithm::calculateChromosomeFitness(Chromosome *chromosome){
    
    double sumOfAccuracies = 0;
    
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
        
        // Accuracy
        double accuracy = 1 - error;
        
        // Add All Accuracies
        sumOfAccuracies += accuracy;
    }
    
    // Set Fitness: Average Accuracy
    double averageAccuracy = sumOfAccuracies/instances.size();
    chromosome->fitnessValue = averageAccuracy;
    
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
            else{
                // Increase chromosomes probability
                chromosome->probability *= (1 + probabilityIncrement);
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

// This method is used to select randomly from array
template<typename Iter> Iter selectRandomly(Iter start, Iter end) {
    
    std::uniform_int_distribution<int> uniformIntDistribution(0, unsigned(std::distance(start, end)) - 1);
    
    std::advance(start, uniformIntDistribution(mt19937));
    
    return start;
}

std::vector<Chromosome *> GeneticAlgorithm::crossover(std::vector<Chromosome *> survivingChromosomes){
    
    std::vector<Chromosome *> offspringChromosomes;
    
    while (1) {
        
        Chromosome *parent1 = *selectRandomly(survivingChromosomes.begin(), survivingChromosomes.end());
        std::vector<double> weights1 = parent1->network->getEdgeWeights();
        std::vector<double> newWeights1;
        
        Chromosome *parent2 = *selectRandomly(survivingChromosomes.begin(), survivingChromosomes.end());
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
        Network *network = chromosome->network;
        std::vector<double> weights = network->getEdgeWeights();
        bool didMutate = false;
        
        for (unsigned j = 0; j < weights.size(); ++j){
            
            if (shouldSelect(mutationProbability)){
                weights[j] = network->randomWeight();
                didMutate = true;
            }
        }
        
        if (didMutate){
            network->setEdgeWeights(weights);
        }
    }
}

void GeneticAlgorithm::logResults(unsigned generationNumber, Chromosome *chromosome){
    
    std::cout << "Generation Number: " + std::to_string(generationNumber) << std::endl;
    
    std::cout << "Fitness Value: " + std::to_string(chromosome->fitnessValue) << std::endl;
    
    std::cout << "Mutation Probability: " + std::to_string(mutationProbability) << std::endl;
    
    std::cout << "Weights: " << std::endl;
    printArray(chromosome->network->getEdgeWeights());
    std::cout << std::endl;
}

void GeneticAlgorithm::printArray(std::vector<double> array){
    
    for (unsigned i = 0; i < array.size(); ++i){
        
        std::ostringstream strs;
        strs << array[i];
        std::string str = strs.str();
        
        std::cout << str + ",";
    }
}
