/*
 *  Created on: 15 March 2021
 *      Author: Lior Sinai
 * Description: prepare a file into a pseudo-token and then into a token
 */

#ifndef TOKENISER_H
#define TOKENISER_H

#include <iomanip> // std::setfill, std::setw
#include <string>
#include <vector>

#include "BlockchainFileSystemException.h"
#include "utilities.h"
#include "sha256/sha256.h"
#include "Token.h"
#include "PseudoToken.h"
#include "JSONwriter.h"

#define MAX_FILE_SIZE 10000000 //10MB

class InvalidTokenException:public BlockchainFileSystemException
{
public:
    InvalidTokenException(const std::string m="Invalid token"):msg(m){}
    const char* what(){return msg.c_str();}
private:
    std::string msg;
};


class Tokeniser
{
public:
    Tokeniser(std::string directory_){
        directory = directory_ +"/";
    }
    Tokeniser(){directory="";}

    Token make_token(PseudoToken pseudoToken);
    void print_token(Token token);
    std::string Tokeniser::token_to_json(Token token, int level=0);
    
    std::vector<uint8_t> serialise(Token token);
    std::string hash_token(Token token);
    bool verify_token(Token token);
private:
    std::string directory;
};

#endif // TOKENISER_H