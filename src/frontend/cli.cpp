/*
 *  Created on: 28 March 2021
 *      Author: Lior Sinai
 * Description: Command line interface for the block chain environment
 */


#include "frontend/cli.h"

using namespace utilities;

void CommandLineInterface::run()
{
    std::cout << "Welcome to the Blockchain file system" << std::endl; 
    CLICommand command;
    /*  get username */
    std::cout <<"what is your username?> ";
    std::string line;
    std::getline(std::cin, line);
    switch_user(line);

    this->help();
    while(1)
    {
        std::cout << this->user << "> ";
        std::getline(std::cin, line);
        if(line == "-q" || line == "--quit" || line == "q" || std::cin.fail()) break;
        command = parse_line(line);
        //std::cout << "option:" << command.argc << ", argument:" << command.argv << ", the rest:" << command.rest<< std::endl; //debugging
        try{
            dispatch_option(command);
        }
        catch (std::exception &e){
            std::cout << "ERROR " << e.what() << std::endl;
            std::cin.get();
        }
        catch (...){
            std::cout << "ERROR " << std::endl;
            std::cin.get();
        }
    }
    std::cout << "exiting program."  << std::endl; 
    std::cout << "Enter y to save blockchain and exit; anything else to exit"  << std::endl; 
    char c = std::cin.get();
    if (c=='y'){
        save_blockchain();
        std::cout <<"Enter any key to exit"  << std::endl;
        std::cin.get();
        std::cin.get();
    }
}

//https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
std::vector<std::string> split(std::string s, std::string delim = " ")
{
    std::vector<std::string> out;
    size_t prev = 0, end = 0;
    do
    {
        end = s.find(delim, prev);
        if (end == std::string::npos) end = s.length();
        std::string token = s.substr(prev, end-prev);
        if (!token.empty()) out.push_back(token);
        prev = end + delim.length();
    }
    while (end < s.length() && prev < s.length());
    return out;
}

CLICommand CommandLineInterface::parse_line(std::string line)
{   
    std::size_t found_dash;
    std::size_t found_space1;
    std::size_t found_space2;
    CLICommand command {};

    found_dash = line.find('-');
    found_space1 = line.find(' ', found_dash);
    found_space1 = std::min(found_space1, line.length());
    if (found_dash !=std::string::npos){
        command.argc = line.substr(found_dash + 1, found_space1-found_dash - 1);\
        if (found_space1 < line.length()) command.argv = split(line.substr(found_space1+1, line.length()));
    }
    if (found_space1 < line.length()){
        command.text = line.substr(found_space1 + 1, line.length()-found_space1);
    }
    
    return command;
}

void CommandLineInterface::dispatch_option(CLICommand command)
{   
    std::string option = command.argc;
    std::string argv   = command.argv.size() > 0 ? command.argv[0] : "";
    if (option.empty()){
        std::cout << "No option entered. For valid options, see -h or --help. q or -q to quit" << std::endl;
    }
    else if (option == "h" || option == "-help"){
        help();
    }
    else if (option == "-header"){
        print_header(argv);
    }
    else if (option == "c" || option == "-commit-block"){
        commit_block();
    }
    else if (option == "i" || option == "-initialise"){
        initialise();
    }
    else if (option == "l" || option == "-load"){
        if (command.argv.size() >= 2){
            load(argv, command.argv[1]);
        }
        else{
            load(argv);
        }
    }
    else if (option == "p" || option == "-print-blockchain"){
        print_blockchain(argv);
    }
    else if (option == "-print-block"){
        print_block(argv);
    }
    else if (option == "-print-token"){
        print_token(argv);
    }
    else if (option == "t" || option == "-stage-token"){
        stage_token();
    }
    else if (option == "s" || option == "-save"){
        save_blockchain();
    }
    else if (option == "-target"){
        set_target(argv);
    }
    else if (option == "-sha256"){
        std:: cout << sha256(command.text) << std::endl;
    }
    else if (option == "-sha256-file"){
        sha256_file(argv);
    }
    else if (option == "-sha256-hex"){
        sha256_hex(argv);
    }
    else if (option == "-switch-user"){
        switch_user(argv);
    }
    else if (option == "u" || option == "-unstage"){
        unstage_block();
    }
    else if (option == "-verify"){
        verify();
    }
    else if (option == "v" ||option == "-version"){
        std::cout << "Blockchain File System v0.1 " << std::endl;
    }
    else if (option == "-demo"){
        run_demo();
    }
    else{
        if (option[0] == '-') option = option.substr(1, option.length());
        std::cout << option << " is not a valid option. For valid options, see -h or --help. q or -q to quit" << std::endl; 
    }
}

void CommandLineInterface::help()
{
    std::cout << "Blockchain File System v0.1 " << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Enter one of the following options:" << std::endl;
    std::cout << "-c | --commit-block             commit a block to the blockchain" << std::endl;
    std::cout << "     --demo                     run a demo" << std::endl;
    std::cout << "-h | --help                     show this help menu" << std::endl;
    std::cout << "     --header [index]           show the raw header for a block" << std::endl;
    std::cout << "-i | --initialise               initialise a new blockchain" << std::endl;
    std::cout << "-l | --load filePath [dirPath]  load the blockchain from a .json file (include extension). dirPath=directory with the blockchain folder" << std::endl;
    std::cout << "-p | --print-blockchain [h]     print the blockchain. If h, print header only" << std::endl;
    std::cout << "     --print-block [index]      print block at index in the blockchain." << std::endl;
    std::cout << "     --print-token [id]         print token with hash=id." << std::endl;
    std::cout << "-q | --quit                     quit" << std::endl;
    std::cout << "-s | --save                     save the blockchain to a file" << std::endl;
    std::cout << "     --sha256 text              calculate the sha256 hash of text (rest of the line)" << std::endl;
    std::cout << "     --sha256-hex hexnum        calculate the sha256 hash of a hexadecimal number" << std::endl;
    std::cout << "     --sha256-file filepath     calculate the sha256 hash of the file" << std::endl;
    std::cout << "     --switch-user username     change the active user" << std::endl;
    std::cout << "-t | --stage-token              stage a token into a block" << std::endl;
    std::cout << "     --target [target]          set the proof of work target (0-256). Display the target if no option" << std::endl;
    std::cout << "-u | --unstage                  delete the last uncommitted block" << std::endl;
    std::cout << "     --verify                   verify the blockchain (excludes uncommited blocks)" << std::endl;
    std::cout << "-v | --version                  print this version" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "| => or, equaivalent commands. [arg] => optional argument" << std::endl;
    std::cout << "If the options [index] or [id] are none, command is executed for the last uncommitted block/token" << std::endl;
    std::cout << "Only one option will be executed per line. Invalid text will be ignored." << std::endl;
}

std::string CommandLineInterface::test_directory_exist(std::string directory)
{
    std::ofstream outFile;
    outFile.open(directory + "/test.txt");
    std::string line;
    if (!outFile)
    {
        do 
        {
            std::cout << "Directory not found. Please make a directory in the current folder called: "<< directory << std::endl;
            std::cout << "Push enter when ready or q to exit." << std::endl;
            std::getline(std::cin, line);
            outFile.open(directory + "/test.txt");
        } while(!outFile && line != "q");
    }
    if (outFile){
        outFile << "Delete me.";
        outFile.close();
    }
    return line;
}

void CommandLineInterface::initialise()
{
    if (this->blockchain){
        std::cout <<"Already initialised the blockchain at /" << this->blockchain->getDirectory() <<". Cannot initialise a new blockchain" << std::endl;
        return;
    }
    std::cout << "Enter a directory name for your blockchain to be stored e.g. MyBlockchain" << std::endl;
    std::cout << "This directory should be empty";
    std::cout << "> ";
    std::string dirName;
    std::getline(std::cin, dirName);
    std::cout << "Your directory name is: " << dirName << std::endl;
    
    std::string line = test_directory_exist(dirName);
    if (line == "q"){
        return;
    }

    blockchain = std::make_unique<Blockchain>(dirName);
    std::string previousHash {"0000000000000000000000000000000000000000000000000000000000000000"};
    this->make_block(0, previousHash, dirName);
}

void CommandLineInterface::make_block(int index, std::string previousHash, std::string directory)
{
    std::cout << "Making a new block ..." << std::endl;

    std::string dirName = directory + "/Block" + std::to_string(index);
    std::string line = test_directory_exist(dirName);
    if (line == "q"){
        return;
    }
    block = std::make_unique<Block>(index, previousHash, directory, (long) std::time(0), this->target, 0);
    std::cout << "done" << std::endl;
    std::cout << "You can now stage tokens (-t) and then commit them (-c)" << std::endl;
}

void CommandLineInterface::load(std::string fileName, std::string dirPath)
{
    if (this->blockchain){
        std::cout <<"Already iniatilised the blockchain at /" << this->blockchain->getDirectory() <<". Cannot load a new file" << std::endl;
        return;
    }
    try{
        this->blockchain = loadBlockchainFromJSON(fileName, dirPath);
    }
    catch (utilities::FileNotFoundException &e){
        std::cerr << e.what() << ". Is the path correct? It cannot have spaces" << std::endl;
        this->blockchain.reset();
    }
    catch (InvalidBlockchainException &e){
        std::cout << e.what() << std::endl;
        this->blockchain.reset();
    }
    catch (std::exception &e){
        std::cout << e.what() << std::endl;
        this->blockchain.reset();
    }
}

void CommandLineInterface::stage_token()
{
    if (!this->blockchain){
        std::cout << notInitialisedMessage << std::endl;
        return;
    }
    if (!this->block){
        std::string previousHash; 
        int index;
        if (blockchain->getHeight() == 0){
             previousHash = "0000000000000000000000000000000000000000000000000000000000000000";
             index = 0;
        }
        else{
            Block prevBlock = this->blockchain->back();
            previousHash = prevBlock.hash();
            index = prevBlock.getIndex() + 1;
        }
        this->make_block(index, previousHash, this->blockchain->getDirectory());
        return;
    }
    std::string filePath;
    std::cout <<"enter a path to a file> ";
    std::getline(std::cin, filePath) ;
    std::string author;
    std::cout <<"enter the author of the file> ";
    std::getline(std::cin, author);

    PseudoToken t = {user, filePath, author};
    
    try{
        token = block->tokeniser->make_token(t);;
    }
    catch (FileNotFoundException &e){
        std::cerr << "ERROR: " << e.what() << std::endl;
        std::cerr << "aborting staging token" << std::endl;
        return;
    }
    catch (const std::exception& e )
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        std::cerr << "aborting staging token" << std::endl;
        return;
    }
    catch(...){
        throw;
    }

    std::cout << "staging token was successful!" << std::endl;
    block->stageToken(token);
    block->tokeniser->print_token(token);;
}

void CommandLineInterface::commit_block()
{
    if (!this->block){
        std::cout << noBlockMessage << std::endl;
        return;
    }
    else if (!this->blockchain) {
        std::cout << notInitialisedMessage << std::endl;
        return;
    }
    std::cout << "attempting to commit block ..." << std::endl;
    try{
        // set target
        this->block->setTarget(target);
        // proof of work
        std::time_t start_time = std::time(0);
        blockchain->commitBlock(*block);
        std::time_t end_time = std::time(0);
        //feedback
        std::cout << "done" << std::endl;
        std::cout << "proof: " << block->getProof() << std::endl;
        std::cout << "time taken: " << end_time - start_time << "s" << std::endl;
        block.reset();
        save_blockchain();
    }
    catch (InvalidBlockException &e){
        std::cerr << e.what() << std::endl;
    }
    catch (InvalidBlockchainException &e){
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception &e){
        std::cerr << e.what() << std::endl;
    }
    catch (...){
        throw;
    }
}

void CommandLineInterface::print_blockchain(std::string print_header)
{
    if (this->blockchain){
        bool print_blocks = !(print_header == "h");
        this->blockchain->print(print_blocks);
    }
    else{
        std::cout << notInitialisedMessage << std::endl;
    }
}

void CommandLineInterface::print_block(std::string index_str)
{
    if (!this->blockchain) {
        std::cout << notInitialisedMessage << std::endl;
        return;
    }
    else if (index_str == ""){
        if (this->block){
            this->block->print();
        }
        else{
            std::cout << noBlockMessage << std::endl;
        }
    }
    else{
        int index;
        try{
            index = std::stoi(index_str);
            this->blockchain->at(index).print();
        }
        catch (std::invalid_argument &e){
            std::cout << "ERROR: " << e.what() << ". Did you enter a number?" << std::endl;
            return;
        }
        catch (std::out_of_range &e){
            std::cout << "ERROR: " << e.what() << ". Please enter a number up to the blockchain height: " << this->blockchain->getHeight() - 1<< std::endl;
            return;
        }
        catch (std::exception &e){
            std::cout << "ERROR: " << e.what() << std::endl;
            return;
        }
  }
}

void CommandLineInterface::print_token(std::string id)
{
    if (!this->blockchain) {
        std::cout << notInitialisedMessage << std::endl;
    }
    if (id.empty()){
        if (!this->block) {
            std::cout << noBlockMessage << std::endl;
        }
        else{
            std::shared_ptr<Tokeniser> tmp_tokeniser = block->tokeniser; 
            tmp_tokeniser->print_token(token);
        }
    }
    else{
        bool found {false};
        for (Block &b : blockchain->getBlocks()){
            std::map<std::string, Token>::iterator itr;
            for (itr = b.getTokens().begin(); itr !=  b.getTokens().end(); ++itr){
                if (itr->first == id){
                    std::cout << "Token is in block " << b.getIndex() << std::endl;
                    b.tokeniser->print_token(itr->second);
                    found = true;
                    break;
                }
            } 
            if (found) break;
        }
        if (!found){
            std::cout << "Token with id=" << id << " was not found" << std::endl;
        }
    }
}

void CommandLineInterface::save_blockchain()
{
    if (!this->blockchain) {
        std::cout << notInitialisedMessage << std::endl;
        return;
    }
    std::string outPath = this->blockchain->getDirectory() + "/" + this->blockchain->getName() + ".json";
    std::cout << "writing to " << outPath << " ...";
    std::ofstream outFile;
    outFile.open(outPath);
    outFile << blockchain->toJSON();
    outFile.close();
    std::cout << "done" << std::endl;
}

void CommandLineInterface::unstage_block()
{
    block.reset();
    std::cout << "unstaged last uncommitted block." << std::endl;
}

void CommandLineInterface::verify()
{
    std::cout << "verifying blockchain ..." << std::endl;
    try{
        blockchain->verify();
        std::cout << "verified!" << std::endl;
    }
    catch (InvalidBlockchainException &e){
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception& e )
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
    catch (...)
    {
        throw;
    }
}

void CommandLineInterface::switch_user(std::string username)
{
    if (!username.empty()){
        user = "@" + username;
    }
    std::cout << "username: " << this->user << std::endl;
}

void CommandLineInterface::sha256_hex(std::string hexnum)
{
    try {
        std::vector<uint8_t> bytes = utilities::hex2byte(hexnum);
        std::string hash = sha256(bytes.data(), bytes.size());
        std::cout << hash << std::endl;
    }
    catch(std::exception &e){
        std::cerr << "ERROR:" << e.what() << std::endl;
    }
}

void CommandLineInterface::sha256_file(std::string filename)
{
    try {
        std::vector<uint8_t> contents;
        bool loaded = loadFile(filename, contents);
        if (!loaded){
            std::cerr << "ERROR: No file loaded. Is the path correct? (no spaces)" << std::endl; 
            return;
        }
        std::string hash = sha256(contents.data(), contents.size());
        std::cout << hash << std::endl;
    }
    catch(std::exception &e){
        std::cerr << "ERROR:" << e.what() << std::endl;
    }
}


void CommandLineInterface::print_header(std::string index_str)
{   
    std::vector<uint8_t> header_bytes;
    std::string header_str;
    std::cout << "version (8) | previousHash (64) | MerkleRoot (64) | timestamp (8) | target (8) | nonce (8)" << std::endl;
    if (!this->blockchain) {
        std::cout << notInitialisedMessage << std::endl;
        return;
    }
    else if (index_str == ""){
        if (this->block){
            header_bytes = this->block->getHeader();
            header_str = utilities::byte2hex(header_bytes);
            std::cout << header_str << std::endl;
        }
        else{
            std::cout << noBlockMessage << std::endl;
        }
    }
    else{
        int index;
        try{
            index = std::stoi(index_str);
            header_bytes = this->blockchain->at(index).getHeader();
            header_str = utilities::byte2hex(header_bytes);
            std::cout << header_str << std::endl;
        }
        catch (std::invalid_argument &e){
            std::cout << "ERROR: " << e.what() << ". Did you enter a number?" << std::endl;
            return;
        }
        catch (std::out_of_range &e){
            std::cout << "ERROR: " << e.what() << ". Please enter a number less than the blockchain height: " << this->blockchain->getHeight() - 1<< std::endl;
            return;
        }
        catch (std::exception &e){
            std::cout << "ERROR: " << e.what() << std::endl;
            return;
        }
  }
}

void CommandLineInterface::set_target(std::string target_str)
{   
    if (!this->blockchain){
        std::cout << notInitialisedMessage << std::endl;
    }
    else if (!target_str.empty()){
        try{
            target = std::stoi(target_str);
            if (target > 256) target = 256;
            if (target < 0)   target = 0;
        }
        catch (std::invalid_argument &e){
            std::cout << "ERROR: " << e.what() << ". Did you enter a number?" << std::endl;
        }
        catch (std::exception &e){
            std::cout << "ERROR: " << e.what() << std::endl;
        }
    }
    std::cout << "current target (0-256): " << this->target << std::endl;
}

void CommandLineInterface::run_demo()
{
    std::cout << "Running the demo" << std::endl;
    std::cout << "Please ensure you the following folder structure in the parent directory:" << std::endl;
    std::cout << "  MyBlockchain" << std::endl;
    std::cout << "     -- Block0" << std::endl;
    std::cout << "  files" << std::endl;
    std::cout << "     -- Hello.txt" << std::endl;
    std::cout << "     -- LoremIpsum.txt" << std::endl;
    std::cout << "     -- FumiakiKawahata_Yoda_LiorSinai.jpg" << std::endl;
    std::cout << "     -- fox.png" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Press any key to start" << std::endl;
    std::cin.get();

    std::string previousHash {"0000000000000000000000000000000000000000000000000000000000000000"};
    std::string blockchainName {"../MyBlockchain"};
    unsigned long int target {0};
    Block block0 = Block(0, previousHash, blockchainName, std::time(0), target, 0);

    std::string user;
    std::string author;
    std::string filePath;
    PseudoToken t;
    Token token;
    user = "@admin";

    // HelloWorld
    filePath = "../files/Hello.txt";
    author   = "Brian Kernighan";
    t = {user, filePath, author};
    try{
        token = block0.tokeniser->make_token(t);
        block0.stageToken(token);
    }
    catch (FileNotFoundException &e){
        std::cerr << e.what() << std::endl;
        std::cerr << "aborting demo" << std::endl;
        return;
    }
    catch (InvalidBlockException &e){
        std::cerr << e.what() << std::endl;
        std::cerr << "aborting demo" << std::endl;
        return;
    }
    catch (std::exception &e){
        std::cerr << e.what() << std::endl;
        std::cerr << "aborting demo" << std::endl;
        return;
    }

    // Yoda
    filePath = "../files/FumiakiKawahata_Yoda_LiorSinai.jpg";
    author = "Lior Sinai";
    t = {user, filePath, author};
    token =  block0.tokeniser->make_token(t);
    block0.stageToken(token);

    // Fox
    filePath = "../files/fox.png";
    author   = "Lior Sinai";
    t = {user, filePath, author};
    token =  block0.tokeniser->make_token(t);
    block0.stageToken(token);

    // LoremImpsum
    filePath = "../files/LoremIpsum.txt";
    author   = "Marcus Tullius Cicero";
    t = {user, filePath, author};
    token =  block0.tokeniser->make_token(t);
    block0.stageToken(token);

    /* blockchain */ 
    std::unique_ptr<Blockchain> blockchain_demo = std::make_unique<Blockchain>("MyBlockchain");
    try{
        blockchain_demo->commitBlock(block0);
    }
    catch (InvalidBlockchainException &e){
        std::cerr << e.what() << std::endl;
        std::cerr << "aborting demo" << std::endl;
        return;
    }
    catch (InvalidBlockException &e){
        std::cerr << e.what() << std::endl;
        std::cerr << "aborting demo" << std::endl;
        return;
    }
    catch (std::exception &e){
        std::cerr << e.what() << std::endl;
        std::cerr << "aborting demo" << std::endl;
        return;
    }

    blockchain_demo->print();

    std::string outPath = "../" + blockchain_demo->getDirectory() + "/" + blockchain_demo->getName() + ".json";
    std::cout << "writing to file " << outPath << " ... ";
    std::ofstream outFile;
    outFile.open(outPath);
    outFile << blockchain_demo->toJSON();
    outFile.close();
    std::cout << "done" << std::endl;
}