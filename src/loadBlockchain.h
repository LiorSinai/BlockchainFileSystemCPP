/*
 *  Created on: 27 March 2021
 *      Author: Lior Sinai
 * Description: Load blockchain from JSON
 */

#ifndef LOAD_BLOCKCHAIN_FROM_JSON
#define LOAD_BLOCKCHAIN_FROM_JSON

#include <string>
#include <vector>
#include <map>

#include "minijson_reader/minijson_reader.hpp"
#include "backend/Blockchain.h"
#include "backend/Block.h"
#include "backend/Token.h"
#include "backend/utilities.h" // for FileNotFoundException

struct PseudoBlock{
        int index;
        std::string previousHash;
        std::string directory;
        long timeStamp; 
        long nonce;
        long target;
        std::map<std::string, Token> tokens;
    };
    
std::unique_ptr<Blockchain> loadBlockchainFromJSON (std::string fileName, std::string pathToDir="");

#endif //LOAD_BLOCKCHAIN_FROM_JSON