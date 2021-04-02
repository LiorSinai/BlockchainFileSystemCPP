/*
 *  Created on: 14 March 2021
 *      Author: Lior Sinai
 * Description: A block for a blockhain
 */

#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <chrono>
#include <map>
#include <set>

#include "BlockchainFileSystemException.h"
#include "Token.h"
#include "Tokeniser.h"
#include "PseudoToken.h"
#include "utilities.h"
#include "MerkleTree.h"

class InvalidBlockException:public BlockchainFileSystemException
{
public:
    InvalidBlockException(const std::string m="Block is invalid"):msg(m){}
    const char* what(){return msg.c_str();}
private:
    std::string msg;
};

class Block
{
public:
    // Constructor 
    Block(int index, std::string previousHash, long target);
    Block(int index, std::string previousHash, std::string directory);
    Block(int index, std::string previousHash, std::string directory, long timeStamp, long target, long nonce);
    
    //General properties of class Block
    const long version = 1;
   
    //functions
    std::vector<uint8_t> getHeader();
    std::string MerkleRoot();
    std::string hash();
    std::shared_ptr<Tokeniser> tokeniser;
    void print();
    void verify();
    std::string Block::toJSON(int level=0);

    //getters
    unsigned long int getIndex(){return this->index;}
    unsigned long int getTarget(){return this->target;}
    unsigned long int getProof(){return this->nonce;}
    bool isVerified(){return this->verified;}

    std::string getPreviousHash(){return this->previousHash;}
    std::time_t getTimeStamp(){return this->timeStamp;}
    std::time_t getNonce(){return this->nonce;}

    std::map<std::string, Token>& getTokens(){return this->tokens;}

    //setters
    void setNonce(unsigned long int nonce){   if (this->tokens.empty()) this->nonce=nonce;}
    void setTarget(unsigned long int target){ if (this->tokens.empty()) this->target=target; else throw BlockchainFileSystemException("Already have tokens in the block");}
    void stageToken(Token token);
private:
    //Attributes of a Block
    unsigned long int index;   //First mined block has index = 0, and so on.
    unsigned long int target;
    std::string directory;

    bool verified {true};
    std::string previousHash;
    std::time_t timeStamp;
    unsigned long int nonce;

    std::map<std::string, Token> tokens;
};

#endif //BLOCK_H