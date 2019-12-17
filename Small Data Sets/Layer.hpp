//
//  Layer.hpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/16/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#ifndef Layer_hpp
#define Layer_hpp

#include <stdio.h>

#include "Node.hpp"

class Layer{
    
public:
    
    std::vector<Node *> nodes;
};

#endif /* Layer_hpp */
