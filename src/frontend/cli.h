/*
 *  Created on: 28 March 2021
 *      Author: Lior Sinai
 * Description: Command line interface for the block chain environment
 */


#ifndef CLI_H
#define CLI_H

#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <stdint.h>
#include <vector>
#include <tuple>

// opensource
#include "sha256/sha256.h"

// local
#include "backend/Blockchain.h"
#include "backend/Block.h"
#include "backend/Tokeniser.h"
#include "backend/MerkleTree.h"
#include "backend/Token.h"
#include "backend/utilities.h"

#include "loadBlockchain.h"
#include "demos.h"

struct CLICommand{
    std::string argc;
    std::vector<std::string> argv;
    std::string text;
};

class CommandLineInterface
{
public:
    // Constructor 
    CommandLineInterface(){};

    void run();

private:
    CLICommand parse_line(std::string line);
    void dispatch_option(CLICommand option);
    std::string test_directory_exist(std::string directory);

    void help();
    void run_demo();
    void commit_block();
    void initialise();
    void print_header(std::string index);
    void load(std::string fileName, std::string dirPath = "");
    void stage_token();
    void print_blockchain(std::string print_header);
    void print_block(std::string index);
    void print_token(std::string id);
    void save_blockchain();
    void set_target(std::string target);
    void sha256_hex(std::string hexnum);
    void sha256_file(std::string filename);
    void switch_user(std::string userName);
    void unstage_block();
    void verify();

    void make_block(int index, std::string previousHash, std::string directory);

    //attributes
    std::string notInitialisedMessage {"Blockchain not initialised. Use --initialise or --load first"}; 
    std::string noBlockMessage {"no block found. Stage a token -t to make a new block."}; 
    std::string user{"@admin"};
    std::unique_ptr<Blockchain> blockchain;
    std::unique_ptr<Block> block;
    Token token;
    long target{0};
};

#endif //CLI_H