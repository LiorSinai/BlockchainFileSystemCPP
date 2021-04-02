#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <stdint.h>
#include <vector>
#include <tuple>

// opensource
#include "sha256/sha256.h"
#include "sha256/sha256.cpp"

// local
#include "backend/Block.h"
#include "backend/Block.cpp"
#include "backend/Token.h"
#include "backend/Tokeniser.h"
#include "backend/Tokeniser.cpp"
#include "backend/utilities.h"
#include "backend/utilities.cpp"
#include "backend/MerkleTree.h"
#include "backend/MerkleTree.cpp"
#include "backend/Blockchain.h"
#include "backend/Blockchain.cpp"

#include "demos.h"
#include "demos.cpp"

#include "loadBlockchain.h"
#include "loadBlockchain.cpp"

#include "frontend/cli.h"
#include "frontend/cli.cpp"

using namespace utilities;

void run_demos();


int main(int argc, char *argv[])
{   
    std::unique_ptr<CommandLineInterface> cli = std::make_unique<CommandLineInterface>();
    cli->run();

    //run_demos();
    // std::cout << "Press any key to exit" << std::endl;
    // std::cin.get();
    
    return 0;
}

void run_demos()
{
    /*test SHA256 */
    //demo_SHA256();

    /*test time */
    //demo_time();

    /* test load file */
    //demo_sha256_file();

    /* test tokeniser */
    //demo_tokeniser();

    /* test block */
    demo_block();

    /* demo bytes */
    //demo_bytes();

    // try{
    //     std::unique_ptr<Blockchain> blockchain = loadBlockchainFromJSON("../MyBlockchain/MyBlockchain.json", "..");
    //     blockchain->print();
    // }
    // catch (utilities::FileNotFoundException &e){
    //     std::cerr << e.what() << std::endl;
    // }
    // catch (InvalidBlockchainException &e){
    //     std::cout << e.what() << std::endl;
    // }
    // catch (std::exception &e){
    //     std::cout << e.what() << std::endl;
    // }
}