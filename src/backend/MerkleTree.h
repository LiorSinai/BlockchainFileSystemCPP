/*
 *  Created on: 14 March 2021
 *      Author: Lior Sinai
 * Description: A block for a blockhain
 */
#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <string>
#include <vector>

#include "sha256/sha256.h"
#include "backend/utilities.h"

class MerkleTree{
public:
    std::string root(std::vector<std::string>& hashes);

private:
    std::string hashLayer(std::vector<std::vector<unsigned char>> layer);
};


#endif //MERKLE_TREE_H