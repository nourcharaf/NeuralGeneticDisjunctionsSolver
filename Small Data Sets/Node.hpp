//
//  Node.hpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/16/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>

#include "Edge.hpp"

class Node{
    
public:
    
    std::vector<Edge *> leftEdges;
    std::vector<Edge *> rightEdges;
    double value;
};

#endif /* Node_hpp */
