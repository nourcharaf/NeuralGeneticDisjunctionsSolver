//
//  Parameters.hpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/18/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#ifndef Parameters_hpp
#define Parameters_hpp

#include <stdio.h>

#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <iterator>
#include <iostream>
#include <sstream>

#include "DataManager.hpp"

// Instances: xor_training.txt, xor_validation.txt, set2_training.txt, set2_validation.txt
static std::vector<std::vector<double>> seenInstances = DataManager::getInstances("xor_training.txt");
static std::vector<std::vector<double>> unseenInstances = DataManager::getInstances("xor_validation.txt");

// Neural Network Parameters
static unsigned numberOfInputs = 3;
static unsigned numberOfOutputs = 1;

// Genetic Algorithm Parameters
static unsigned populationSize = 1000;
static unsigned numberOfGenerations = 20;
static double desiredAccuracy = 0.9;
static double elitePercentage = 0.1;
static double survivingPercentage = 0.1;
static double crossoverProbability = 0.5;
static double maxMutationProbability = 1;
static double mutationDecrement = 0.02;
static double probabilityIncrement = 0.01;
static unsigned uniformIntDistributionSize = 1000;

// Cross Validation Parameters
static double crossValidationFolds = 2;

// Hyper Genetic Algorithm Parameters
static unsigned hyperPopulationSize = 1000;
static unsigned hyperNumberOfGenerations = 20;
static double hyperDesiredAccuracy = 0.9;
static double hyperElitePercentage = 0.1;
static double hyperSurvivingPercentage = 0.1;
static double hyperCrossoverProbability = 0.5;
static double hyperMaxMutationProbability = 1;
static double hyperMutationDecrement = 0.02;
static double hyperProbabilityIncrement = 0.01;
static unsigned hyperUniformIntDistributionSize = 1000;

#endif /* Parameters_hpp */
