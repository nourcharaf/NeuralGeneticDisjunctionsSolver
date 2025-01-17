//
//  DataManager.hpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/17/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#ifndef DataManager_hpp
#define DataManager_hpp

#include <stdio.h>

#include <fstream>
#include <sstream>
#include <random>
#include <vector>

class DataManager{
    
public:
    
    DataManager();
    static std::vector<std::vector<double>> getInstances(std::string fileName);
    static void generateXORData(std::string fileName,unsigned instances,unsigned width);
    
};

#endif /* DataManager_hpp */
