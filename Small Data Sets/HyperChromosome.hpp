//
//  HyperChromosome.hpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/18/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#ifndef HyperChromosome_hpp
#define HyperChromosome_hpp

#include <stdio.h>

#include "CrossValidation.hpp"

class HyperChromosome{
    
public:
    
    CrossValidation *crossValidation;
    double fitnessValue;
    double probability;
    
    HyperChromosome();
};

#endif /* HyperChromosome_hpp */
