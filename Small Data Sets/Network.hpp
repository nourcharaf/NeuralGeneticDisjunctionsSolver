//
//  Network.hpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/16/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#ifndef Network_hpp
#define Network_hpp

#include <stdio.h>

#include <cmath>

#include "Layer.hpp"

class Network{
    
public:
    
    enum TransferFunction {
        hyperbolicTangent,
        fastSigmoid
    };
    
    static std::vector<TransferFunction> getTransferFunctions();
    
    double randomRange;
    unsigned transferFunction;
    std::vector<unsigned> topology;
    std::vector<Layer *> layers;
    
    Network();
    void createLayers();
    void setRandomEdgeWeights();
    double randomWeight();
    void setEdgeWeights(std::vector<double>weights);
    std::vector<double> getEdgeWeights();
    void feedForward(std::vector<double> inputValues);
    double applyTransferFunction(double x);
    std::vector<double> getOutputs();
};

#endif /* Network_hpp */
