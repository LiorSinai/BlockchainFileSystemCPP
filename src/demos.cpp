#include "demos.h"

using namespace utilities;

void demo_SHA256(){ 
    std::string input = "Hello, how are you?";
    std::string hash1 = "0x";
    hash1.append(sha256(input));
    std::cout << "sha256('"<< input << "'):" << hash1  <<  std::endl;
    std::cout << "sha256('"<< input << "'):" << hex2bin(hash1)  <<  std::endl;
    std::vector<unsigned char> hashbytes = hex2byte(hash1);
    std::stringstream ss;
    ss << "sha256('"<< input << "'):";
    for (unsigned char c: hashbytes){
        ss << " " <<(int)c;
    }
    std::cout << ss.str() << std::endl;

    std::cout << "sha256(abcdefghi):" << hex2bin("abcdefghi")  <<  std::endl;


}

void demo_time(){
    std::time_t end_time = std::time(0);
    tm* gmtm = gmtime(&end_time);
    std::cout << "finished computation at " << std::asctime(gmtm)  <<  std::endl;
}

void demo_bytes(){
    std::time_t the_time = std::time(0);
    std::cout << std::ctime(&the_time) << "seconds:" << the_time << std::endl;
    std::vector<uint8_t> bytes = utilities::long2byte(the_time);
    for (int b: bytes)
        std::cout << b << " ";
    std::cout << std::endl;
    std::cout << utilities::byte2long(bytes) << std::endl;

    try {
        std::vector<uint8_t> bytes = utilities::hex2byte("ba") ;
        for (int b: bytes)
            std::cout << (int) b << " ";
        std::cout << std::endl;
    }
    catch (const std::exception e){
        std::cout << e.what() << std::endl;
    }

}



void demo_sha256_file(){
    std::string filename = "../files/FumiakiKawahata_Yoda_LiorSinai.jpg"; //"files/FumiakiKawahata_Yoda_LiorSinai.jpg";
    // std::ifstream stream(filename, std::ios::binary);
    // std::vector<uint8_t> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    // stream.close();

    std::vector<uint8_t> contents;
    loadFile(filename, contents);

    long fileSize = getFileSize(filename);    
    std::cout << "file size: " << fileSize << "; content size: " << contents.size() << std::endl;

    /* make hash */
    std::string hash1 = sha256(contents.data(), contents.size());
    std::cout << "sha256('"<< filename << "'):" << hash1  <<  std::endl;

    /*write to output file */
    /*this doubles the file size, because each byte is write as 2 characters*/
    std::ofstream outFile("example.txt");
    outFile << "0x";
    for (const auto &e : contents){
        outFile << std::setfill('0') << std::hex << std::setw(2) << (int)e;
    }
    outFile.close();
}

void demo_tokeniser(){
    std::string filename = "../files/LoremIpsum.txt";
    std::string user     = "@LiorSinai";
    std::string author   = "Lior Sinai";

    std::unique_ptr<Tokeniser> tokeniser = std::make_unique<Tokeniser>();
    PseudoToken t = {user, filename, author};
    Token token = tokeniser->make_token(t);

    tokeniser->print_token(token);
    std::cout << tokeniser->token_to_json(token) << std::endl;
    for (int b: tokeniser->serialise(token))
        std::cout << b << " ";
    std::cout << std::endl;
    std::cout << tokeniser->hash_token(token) << std::endl;

   
    std::cout << "verifying token ... " ;
    std::cin.get();
    try{
        std::cout << tokeniser->verify_token(token) << std::endl;
    }
    catch (InvalidTokenException &e ){
        std::cout << "invalid token: " << e.what() << std::endl;
    }
    catch (std::exception &e ){
        std::cout << e.what() << std::endl;
    }
}

void demo_block(){
    std::string previousHash {"0000000000000000000000000000000000000000000000000000000000000000"};
    std::string blockchainName {"../MyBlockchain"};
    unsigned long int target {0};
    Block block0 = Block(0, previousHash, blockchainName, std::time(0), target, 0);

    // put tokens in the block chain
    std::string user;
    std::string author;
    std::string filePath;
    PseudoToken t;
    Token token;
    user = "@LiorSinai";

    // HelloWorld
    filePath = "../files/Hello.txt";
    author   = "Brian Kernighan";
    t = {user, filePath, author};
    try{
        token = block0.tokeniser->make_token(t);
        block0.stageToken(token);
    }
    catch (InvalidBlockException &e){
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception &e){
        std::cerr << e.what() << std::endl;
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

    // block
    block0.print();

    std::vector<uint8_t> header = block0.getHeader();
    for (int b: header)
        std::cout << b << " ";
    std::cout << std::endl;
    std::cout << header.size() << std::endl;

    std::cout << block0.toJSON() << std::endl;

    std::cout << "verifying block ... " ;
    std::cin.get();
    try{
        block0.verify();
    }
    catch (InvalidBlockException &e ){
        std::cerr << "invalid block:" << e.what() << std::endl;
    }
    catch (std::exception &e ){
        std::cerr << e.what() << std::endl;
    }
    std::cout << block0.isVerified() << std::endl;

    /* blockchain */ 
    std::unique_ptr<Blockchain> blockchain = std::make_unique<Blockchain>("MyBlockchain");
    try{
        blockchain->commitBlock(block0);
    }
    catch (InvalidBlockException &e){
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception &e){
        std::cerr << e.what() << std::endl;
    }
    /* block 1: empty */
    previousHash = block0.hash();
    Block block1 = Block(1, previousHash, blockchainName, std::time(0), target, 0);
    blockchain->commitBlock(block1);

    blockchain->print();
    std::cout << "verifying blockchain ... " ;
    std::cin.get();

    try{
        blockchain->verify();
    }
    catch (InvalidBlockchainException &e){
        std::cerr << "invalid blockchain: " << e.what() <<  std::endl;
    }
    std::cout << blockchain->isVerified() << std::endl;

    std::string outPath = "../" + blockchain->getDirectory() + "/" + blockchain->getDirectory() + ".json";
    std::cout << "writing to file " << outPath << " ... ";
    std::ofstream outFile;
    outFile.open(outPath);
    outFile << blockchain->toJSON();
    outFile.close();
    std::cout << "done" << std::endl;
}