/*
 *  Created on: 14 March 2021
 *      Author: Lior Sinai
 * Description: A block for a blockhain
 */

#include "block.h"

Block::Block(int index_, std::string previousHash_, long target_)
{
    this->index = index_;
    this->target = target_;
    this->previousHash = previousHash_;
    // set defaults
    this->timeStamp = std::time(0);
    this->nonce = 0;
    this->tokeniser = std::make_shared<Tokeniser>("Block" +  std::to_string(this->getIndex()));
}

Block::Block(int index_, std::string previousHash_, std::string directory_)
{
    this->index = index_;
    this->previousHash = previousHash_;
    this->directory = directory_;
    // set defaults
    this->timeStamp = std::time(0);
    this->target = 1;
    this->nonce = 0;
    this->tokeniser = std::make_shared<Tokeniser>(directory_ + "/Block" +  std::to_string(this->getIndex()));
}

Block::Block(int index_, std::string previousHash_, std::string directory_, long timeStamp_, long target_, long nonce_)
{
    this->index = index_;
    this->target = target_;
    this->previousHash = previousHash_;
    this->directory = directory_;
    this->timeStamp = timeStamp_;
    this->nonce = nonce_;
    // set defaults
    this->tokeniser = std::make_shared<Tokeniser>(directory_ + "/Block" +  std::to_string(this->getIndex()));
}


void Block::stageToken(Token token)
{
    std::string hash = this->tokeniser->hash_token(token);
    this->tokens.insert(std::pair<std::string, Token>(hash, token));
}

void Block::print()
{   
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "\n";
    std::cout << this->directory << "/Block" << this->getIndex() << std::endl;

    std::time_t blocktime = this->getTimeStamp();
    struct tm* timeinfo = gmtime (& blocktime);
   
    std::cout << "version:       " << this->version << std::endl;
    std::cout << "previous hash: " << this->getPreviousHash() << std::endl;
    std::cout << "this hash:     " << this->hash() << std::endl;
    std::cout << "Merkle root:   " << this->MerkleRoot() << std::endl;
    std::cout << "timestamp:     " << std::asctime(timeinfo);
    std::cout << "target:        " << this->getTarget() << std::endl;
    std::cout << "nonce:         " << this->getNonce() << std::endl;
    std::cout << "\n";

    std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << std::endl;
    //std::cout << "-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - -  -  -  -  -  -  -  -  -  -  -  -  -  -  -" << std::endl;
    std::cout << "\n";
    std::map<std::string, Token>::iterator itr; 
    for (itr = this->tokens.begin(); itr != this->tokens.end(); ++itr) { 
        std::cout << "Token " << itr -> first << std::endl;
        this->tokeniser->print_token(itr->second);
    } 
}

std::string Block::toJSON(int level)
{   
    std::unique_ptr<JSONwriter> writer = std::make_unique<JSONwriter>(level);
    std::string directory = "Block" + std::to_string(this->getIndex());
    writer->write("directory", directory);
    //writer->write("version", this->version);
    writer->write("previousHash", this->previousHash);
    writer->write("MerkleRoot", this->MerkleRoot());
    writer->write("timeStamp", (long) this->timeStamp);
    writer->write("target", this->target);
    writer->write("nonce", this->nonce);

    // make json objects from the map
    std::map<std::string, Token>::iterator itr; 
    std::map<std::string, std::string> tokens_json; 
    for (itr = this->tokens.begin(); itr != this->tokens.end(); ++itr){
        std::string json = tokeniser->token_to_json(itr->second, level+4);
        tokens_json.insert(std::pair<std::string, std::string>(itr->first, json));
    }
    writer->write_map("tokens", tokens_json, "id", "token");
    return writer->result();
}

std::string Block::MerkleRoot()
{   
    std::map<std::string, Token>::iterator itr;
    std::vector<std::string> keys;
    keys.reserve(this->tokens.size());
    for (itr = this->tokens.begin(); itr != this->tokens.end(); ++itr) { 
        keys.push_back(itr -> first);
    } 
    MerkleTree tree = MerkleTree();
    return tree.root(keys);
}

std::vector<uint8_t> Block::getHeader()
{
    std::vector<uint8_t> header; // version previousHash MerkleRoot timestamp target nonce
    std::vector<uint8_t> versionBytes = utilities::long2byte(this->version);
    header.insert(header.end(), versionBytes.begin(), versionBytes.end());
    std::vector<uint8_t> previousHashBytes = utilities::hex2byte(this->previousHash);
    header.insert(header.end(), previousHashBytes.begin(), previousHashBytes.end());
    std::vector<uint8_t> rootBytes = utilities::hex2byte(this->MerkleRoot());
    header.insert(header.end(), rootBytes.begin(), rootBytes.end());
    std::vector<uint8_t> timeStampBytes = utilities::long2byte(this->timeStamp);
    header.insert(header.end(), timeStampBytes.begin(), timeStampBytes.end());
    std::vector<uint8_t> targetBytes = utilities::long2byte(this->target);
    header.insert(header.end(), targetBytes.begin(), targetBytes.end());
    std::vector<uint8_t> nonceBytes = utilities::long2byte(this->getNonce());
    header.insert(header.end(), nonceBytes.begin(), nonceBytes.end());
    
    return header;
}

std::string Block::hash()
{
    std::vector<unsigned char> header = this->getHeader();
    return sha256(header.data(), header.size());
}

void Block::verify(){
    std::map<std::string, Token>::iterator itr; 
    std::string msg_block = "\nBlock " + std::to_string(this->getIndex()) + " not verfied";
    for (itr = this->tokens.begin(); itr != this->tokens.end(); ++itr) { 
        try{
            tokeniser->verify_token(itr->second);
        }
        catch (InvalidTokenException& e){
            this->verified=false;
            throw InvalidBlockException(e.what() + msg_block);
            return;
        }
        catch (utilities::FileNotFoundException& e){
            this->verified=false;
            throw InvalidBlockException(e.what() + msg_block);
            return;
        }
        catch (std::exception& e){
            this->verified=false;
            throw InvalidBlockException(e.what() + msg_block);
            return;
        }
        std::string tokenHash = this->tokeniser->hash_token(itr->second);
        if (tokenHash != itr->first){
            std::string msg = std::string("Token hashes do not match:\nOriginal:   ") + itr->first + std::string("\nCalculated: ") + tokenHash;
            this->verified=false;
            throw InvalidBlockException(msg + msg_block);
        }   
    }

    this->verified=true;
}

