//
//  DataManager.cpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/17/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#include "DataManager.hpp"

DataManager::DataManager(){
    
}

std::vector<std::vector<double>> DataManager::getInstances(std::string fileName){
    
    std::vector<std::vector<double>> instances;
    
    std::ifstream file(fileName);
    
    std::string line;
    
    while(std::getline(file,line)){
        
        std::stringstream linestream(line);
        std::string value;
        
        std::vector<double> allLineValues;
        
        while(std::getline(linestream,value,',')){
            allLineValues.push_back(std::stod(value));
        }
        instances.push_back(allLineValues);
    }
    
    return instances;
}

void DataManager::generateXORData(std::string fileName,unsigned instances){
    
    std::ofstream file(fileName);
    
    for (unsigned i = 0; i < instances; ++i){
        
        int x1 = rand()/double(RAND_MAX) + 0.5;
        int x2 = rand()/double(RAND_MAX) + 0.5;
        int x3 = rand()/double(RAND_MAX) + 0.5;
        
        int output = x1 != x2;
        output = output != x3;
        
        file << std::to_string(x1) + "," + std::to_string(x2) + "," + std::to_string(x3) + "," + std::to_string(output) + "\n";
    }
    
    file.close();
}
