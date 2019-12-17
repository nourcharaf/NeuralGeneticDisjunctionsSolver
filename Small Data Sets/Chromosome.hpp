//
//  Chromosome.hpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/16/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#ifndef Chromosome_hpp
#define Chromosome_hpp

#include <stdio.h>

#include "Network.hpp"

class Chromosome{
    
public:
    
    Network *network;
    double fitnessValue;
    double probability;
    
    Chromosome();
    void setFitnessValue(double fitnessValue);
    void setProbability(double probability);
};

#endif /* Chromosome_hpp */
