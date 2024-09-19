//
//  Network.cpp
//  Small Data Sets
//
//  Created by Nour Charaf on 12/16/19.
//  Copyright © 2019 Apple. All rights reserved.
//

#include "Network.hpp"
#include "assert.h"

Network::Network(){
    
}

std::vector<Network::TransferFunction> Network::getTransferFunctions(){
    return {hyperbolicTangent,fastSigmoid};
}

void Network::createLayers(){
    
    // Empty Layers
    layers.clear();
    
    // Add layers and nodes
    
    for (unsigned i = 0; i < topology.size(); ++i){
        
        Layer *layer = new Layer();
        layers.push_back(layer);
        
        unsigned numberOfNodes = topology[i];
        
        for (unsigned j = 0; j < numberOfNodes; ++j){
            
            Node *newNode = new Node();
            layer->nodes.push_back(newNode);
        }
        
        // Bias Node
        Node *biasNode = new Node();
        layer->nodes.push_back(biasNode);
        
        biasNode->value = 1;
    }
    
    // Add edges
    
    for (unsigned i = 0; i < layers.size() - 1; ++i){
        
        Layer *leftLayer = layers[i];
        Layer *rightLayer = layers[i + 1];
        
        for (unsigned j = 0; j < leftLayer->nodes.size(); ++j){
            
            Node *leftNode = leftLayer->nodes[j];
            
            for (unsigned k = 0; k < rightLayer->nodes.size(); ++k){
                
                Node *rightNode = rightLayer->nodes[k];
                
                Edge *edge = new Edge();
                
                leftNode->rightEdges.push_back(edge);
                rightNode->leftEdges.push_back(edge);
                
                edge->leftNode = leftNode;
                edge->rightNode = rightNode;
            }
        }
    }
}

void Network::setRandomEdgeWeights(){
    
    for (unsigned i = 0; i < layers.size(); ++i){
        
        Layer *layer = layers[i];
        
        for (unsigned j = 0; j < layer->nodes.size(); ++j){
            
            Node *node = layer->nodes[j];
            
            for (unsigned k = 0; k < node->rightEdges.size(); ++k){
                
                Edge *rightEdge = node->rightEdges[k];
                rightEdge->weight = randomWeight();
            }
        }
    }
}

double Network::randomWeight(){
    return ((rand()/double(RAND_MAX)) * 2 * randomRange) - randomRange; // (rand()/double(RAND_MAX)); ((rand()/double(RAND_MAX)) * 2) - 1;
}

void Network::setEdgeWeights(std::vector<double>weights){

    unsigned l = 0;
    
    for (unsigned i = 0; i < layers.size(); ++i){
        
        Layer *layer = layers[i];

        for (unsigned j = 0; j < layer->nodes.size(); ++j){

            Node *node = layer->nodes[j];

            for (unsigned k = 0; k < node->rightEdges.size(); ++k){

                Edge *rightEdge = node->rightEdges[k];
                rightEdge->weight = weights[l];
                l++;
            }
        }
    }
}

std::vector<double> Network::getEdgeWeights(){
    
    std::vector<double> weights;
    
    for (unsigned i = 0; i < layers.size(); ++i){
        
        Layer *layer = layers[i];
        
        for (unsigned j = 0; j < layer->nodes.size(); ++j){
            
            Node *node = layer->nodes[j];
            
            for (unsigned k = 0; k < node->rightEdges.size(); ++k){
                
                Edge *rightEdge = node->rightEdges[k];
                weights.push_back(rightEdge->weight);
            }
        }
    }
    
    return weights;
}

void Network::feedForward(std::vector<double> inputValues){
    
    // Quick check
    assert(layers[0]->nodes.size() - 1 == inputValues.size());
    
    // Latch input values to input nodes
    Layer *inputLayer = layers[0];
    
    for (unsigned i = 0; i < inputLayer->nodes.size() - 1; ++i){
        
        double inputValue = inputValues[i];
        
        Node *inputNode = inputLayer->nodes[i];
        inputNode->value = inputValue;
    }
    
    // Forward Propagate
    for (unsigned i = 1; i < layers.size(); ++i){
        
        Layer *layer = layers[i];
        
        for (unsigned j = 0; j < layer->nodes.size() - 1; ++j){
            
            Node *node = layer->nodes[j];
            
            double sum = 0;
            
            for (unsigned k = 0; k < node->leftEdges.size(); ++k){
                
                Edge *leftEdge = node->leftEdges[k];
                
                Node *leftNode = leftEdge->leftNode;
                
                sum += leftNode->value * leftEdge->weight;
            }
            
            node->value = applyTransferFunction(sum);
        }
    }
}

double Network::applyTransferFunction(double x){
    
    if (transferFunction == hyperbolicTangent){
        return tanh(x);
    }
    else if (transferFunction == fastSigmoid){
        return x/(1+abs(x));
    }
    else{
        return 0;
    }
}

std::vector<double> Network::getOutputs(){
    
    std::vector<double> outputs;
    
    Layer *lastLayer = layers.back();
    
    for (unsigned i = 0; i < lastLayer->nodes.size(); ++i){
        
        Node *node = lastLayer->nodes[i];
        outputs.push_back(node->value);
    }
    
    return outputs;
}
