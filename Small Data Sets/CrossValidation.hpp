//
//  CrossValidation.hpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/18/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#ifndef CrossValidation_hpp
#define CrossValidation_hpp

#include <stdio.h>

#include "DataManager.hpp"
#include "GeneticAlgorithm.hpp"

class CrossValidation{
    
public:
    
    CrossValidation();
    void start();
    void kFoldCrossValidation();
    void trainWithInstances(GeneticAlgorithm *geneticAlgorithm, std::vector<std::vector<double>> instances);
    void testWithInstances(GeneticAlgorithm *geneticAlgorithm, std::vector<std::vector<double>> instances);
    void reportFinalResults(std::vector<GeneticAlgorithm *> geneticAlgorithms);
    void printArray(std::vector<double> array);
};

#endif /* CrossValidation_hpp */
