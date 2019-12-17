//
//  Chromosome.cpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/16/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#include "Chromosome.hpp"

Chromosome::Chromosome(){
    
}

void Chromosome::setFitnessValue(double fitnessValue){
    this->fitnessValue = fitnessValue;
}

void Chromosome::setProbability(double probability){
    this->probability = probability;
}
