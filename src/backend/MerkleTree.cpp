/*
 *  Created on: 14 March 2021
 *      Author: Lior Sinai
 * Description: A simple Merkle Tree implementation
 * 
 * sources: 
 *  - https://en.wikipedia.org/wiki/Merkle_tree
 *  - https://101blockchains.com/merkle-trees/ 
 */

#include "MerkleTree.h"

std::string MerkleTree::root(std::vector<std::string>& hashes)
{   
    std::string newHash;

    std::vector<std::vector<unsigned char>> layer;
    for (std::string hash: hashes){
        layer.push_back(utilities::hex2byte(hash));
    }
    if (layer.size() == 0){
        return sha256(std::string(""));
    }
    return hashLayer(layer);
}

std::string MerkleTree::hashLayer(std::vector<std::vector<unsigned char>> layer)
{
    if (layer.size() == 1){//done
        return utilities::byte2hex(layer[0]);
    }
    std::vector<std::vector<unsigned char>> newlayer;
    newlayer.reserve(layer.size()/2);
    for (int i {0}; i < (layer.size() - 1); i = i + 2)
    {
        std::vector<unsigned char> node(layer[i]);
        node.insert(node.end(), layer[i+1].begin(), layer[i+1].end() );
        std::string hashhex = sha256(&node[0], node.size());
        std::vector<unsigned char> hashbyte = utilities::hex2byte(hashhex);
        newlayer.push_back(hashbyte);
    }
     if (layer.size() % 2 == 1)
    {
        //odd number of nodes, therefore the last node needs to be paired with itself
        //
        int i = layer.size() - 1;
        std::vector<unsigned char> node(layer[i]);
        node.insert(node.end(), layer[i].begin(), layer[i].end() );
        std::string hashhex = sha256(&node[0], node.size());
        std::vector<unsigned char> hashbyte = utilities::hex2byte(hashhex);
        newlayer.push_back(hashbyte);
    }
    return hashLayer(newlayer);
}