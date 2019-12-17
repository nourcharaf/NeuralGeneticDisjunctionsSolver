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

#include <vector>

class Node;

class Edge{
    
public:
    
    Node *leftNode;
    Node *rightNode;
    double weight;
};

#endif /* Edge_hpp */
