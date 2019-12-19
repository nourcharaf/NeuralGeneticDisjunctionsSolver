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

void DataManager::generateXORData(std::string fileName,unsigned instances,unsigned width){
    
    std::ofstream file(fileName);
    
    for (unsigned i = 0; i < instances; ++i){
        
        std::string output = "";
        unsigned result = 0;
        
        for (unsigned j = 0; j < width; ++j){
            
            int x = rand()/double(RAND_MAX) + 0.5;
            
            output = output + std::to_string(x) + ",";
            
            result = j == 0 ? x : result != x;
        }
        
        output = output + std::to_string(result);
        
        file << output + "\n";
    }
    
    file.close();
}
