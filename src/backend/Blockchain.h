/*
 *  Created on: 21 March 2021
 *      Author: Lior Sinai
 * Description: A blockchain
 */

#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>
#include <iostream>
#include <chrono>


#include "Block.h"
#include "utilities.h"
#include "sha256/sha256.h"
#include "JSONwriter.h"

class InvalidBlockchainException:public BlockchainFileSystemException
{
public:
    InvalidBlockchainException(const std::string m="Blockchain is invalid"):msg(m){}
    const char* what(){return msg.c_str();}
private:
    std::string msg;
};

class Blockchain
{
public:
    // Constructor 
    Blockchain(){this->directory="Blockchain"; this->timeStamp = std::time(0);}
    Blockchain(std::string directory_) {
        this->directory=directory_; 
        std::size_t found   = directory_.find_last_of("/\\");
        if (found == std::string::npos) found = -1;
        this->name =directory_.substr(found+1); 
        this->timeStamp = std::time(0);};
    Blockchain(std::string directory_, std::time_t t) 
        {this->directory=directory_; 
        std::size_t found   = directory_.find_last_of("/\\");
        if (found == std::string::npos) found = -1;
        this->name =directory_.substr(found+1); 
        this->timeStamp = t;};
    const long version = 1;

    //functions
    Block makeBlock();
    void commitBlock(Block& block, bool doProofofWork=true);
    void print(bool print_blocks=true);
    void verify();
    bool isVerified(){return this->verified;}
    std::string toJSON(int level=0);
    bool is_valid_proof(std::string const & s, int target);
    bool is_valid_proof(unsigned char (& bytes)[SHA256::DIGEST_SIZE], int target);

    unsigned long int proofOfWork(std::vector<uint8_t> bytes,  unsigned long int target=0, unsigned long int start=0);

    //getters
    Block& front(){return this->blocks.front();}
    Block& back() {return this->blocks.back();}
    Block& at(int index) {return this->blocks.at(index);}
    std::vector<Block>& getBlocks() {return this->blocks;}
    int getIndex() {return this->blocks.size() -1;}
    int getHeight() {return this->blocks.size();}
    const std::time_t getTimeStamp(){return this->timeStamp ;}
    std::string getDirectory() {return this->directory;}
    std::string getName() {return this->name;}

    //setters
    void setDirectory(std::string directory_){this->directory = directory_;}

private:
    std::vector<Block> blocks;
    bool verified{true};
    std::time_t timeStamp;
    std::string directory;
    std::string name;
};

#endif // BLOCKCHAIN_H