//
//  Edge.hpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/16/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#ifndef Edge_hpp
#define Edge_hpp

#include <stdio.h>

#include "Parameters.hpp"

class Node;

class Edge{
    
public:
    
    Node *leftNode;
    Node *rightNode;
    double weight;
    
    Edge();
};

#endif /* Edge_hpp */
