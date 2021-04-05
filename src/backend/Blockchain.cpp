/*
 *  Created on: 21 March 2021
 *      Author: Lior Sinai
 * Description: A blockchain
 */

#include "Blockchain.h"
using namespace utilities;

Block Blockchain::makeBlock()
{
    std::string previousHash;
    if (this->blocks.size()==0){
        previousHash = "0x0000000000000000000000000000000000000000000000000000000000000000";

    }
    else{
        std::vector<unsigned char> header = this->back().getHeader();
        previousHash = sha256(header.data(), header.size());
    }
    return Block(this->getIndex() + 1, previousHash, this->directory, std::time(0), 0, 0);
}


void Blockchain::commitBlock(Block& block, bool doProofofWork)
{   
    bool valid = true;
    std::string msg;
    try{
        this->verify();
    }
    catch(InvalidBlockchainException &e){
        throw e;
    }
    catch(...){
       throw InvalidBlockchainException("blockchain is not verified"); 
    }
    // checks
    if (block.getIndex() != this->getIndex() + 1){
        msg = std::string("Block is not valid: index must follow last index in the blockchain: " + 
                            std::to_string(block.getIndex()) + " != " + std::to_string(this->getIndex() + 1));
        throw InvalidBlockException(msg);
    }
    if (block.getIndex() > 0){
        std::string previousHash = this->back().hash();
        if (block.getPreviousHash() != previousHash){
            msg = std::string("Previous block hashes do not match:\nProposed block:   ") + block.getPreviousHash() + 
                                                     std::string("\nBlockchain block: ") + previousHash;
            throw InvalidBlockException("Block is not valid: " + msg);
        }
    }
    if (block.version != this->version){
        msg = std::string("Block is not valid: versions do not match: ") + std::to_string(block.version) + " != " + std::to_string(this->version);
        throw InvalidBlockException(msg);
    }
    if (block.getTimeStamp() > std::time(0)){
        std::time_t blocktime = block.getTimeStamp();
        struct tm* timeinfo = gmtime (& blocktime);
        msg = std::string("Block is not valid: creation time is in the future: ") + std::asctime(timeinfo);
        throw InvalidBlockException(msg);
    }
    unsigned long proof = 0;
    if (doProofofWork) {proof = proofOfWork(block.getHeader(), block.getTarget(), block.getNonce());}
    block.setNonce(proof);
    this->blocks.push_back(block);
}


void Blockchain::print(bool print_blocks)
{   
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "\n";
    std::cout << "Blockchain VERSION " << std::to_string(this->version) << std::endl;

    std::time_t blocktime = this->getTimeStamp();
    struct tm* timeinfo = gmtime (& blocktime);

    std::cout << "timestamp UTC: " << std::asctime(timeinfo);
    std::cout << "name:          " << this->name << std::endl;
    std::cout << "directory:     " << this->directory << std::endl;
    std::cout << "height:        " << std::to_string(this->blocks.size()) << std::endl;
    std::cout << "\n";

    if (print_blocks){
        for (Block &block: this->blocks){
            block.print();        
        }
    }
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
}

std::string Blockchain::toJSON(int level)
{
    std::unique_ptr<JSONwriter> writer = std::make_unique<JSONwriter>(level);
    writer->write("directory",  this->name);
    writer->write("version", this->version);
    writer->write("timeStamp", (long) this->timeStamp);
    // make array of blocks
    std::vector<std::string> blocks_json;
    for (Block &b: this->blocks){
        blocks_json.push_back(b.toJSON(2));
    }
    writer->write_array("blocks", blocks_json);
    return writer->result();
}

void Blockchain::verify(){
    std::string msg_blockchain = "\nBlockchain not verified";
    for (int i{0}; i < this->getHeight(); i++){
        try{
            blocks[i].verify();
        }
        catch (InvalidBlockException &e){
            this->verified=false;
            throw InvalidBlockchainException(e.what() + msg_blockchain);
            return;
        }
        catch (std::exception &e){
            this->verified=false;
            throw InvalidBlockchainException(e.what() + msg_blockchain);
            return;
        }
        if (i<this->getHeight() -1 ){
            std::string blockHash = this->blocks[i].hash();
            if (blockHash != blocks[i+1].getPreviousHash()){
                std::string msg = std::string("Block hashes do not match:\nOriginal:   ") + blocks[i+1].getPreviousHash() + std::string("\nCalculated: ") + blockHash;
                this->verified=false;
                throw InvalidBlockchainException(msg + msg_blockchain);
            }
        }
    }
}


inline bool starts_with_n(std::string const & s, int n, char target)
{
    n = n > s.length() ? s.length() : n;
    for (int i {0}; i < n; i++){
        if (s[i] != target) return false;
    }   
    return true;
} 

bool Blockchain::is_valid_proof(std::string const & s, int target)
{
    if (starts_with_n(s, target/4, '0')){
        int idx = target/4; // a hex char has 4 bits
        int rem = target - idx*4 ;
        if (rem == 0){
            return true;
        }
        else if (rem==1){
            return s[idx] < '8'; // hex2bin(7) = 0111
        }
        else if (rem==2){
            return s[idx] < '4'; // hex2bin(3) = 0011
        }
        else if (rem==3){
            return s[idx] < '2'; // hex2bin(1) = 0001
        }
    }
    return false;
} 


bool Blockchain::is_valid_proof(unsigned char (& bytes)[SHA256::DIGEST_SIZE], int target)
{
    int idx = target/8; //a byte has 8 bits
    int rem = target - idx*8 ;
    for (int i {0}; i < idx; i++){
        if (bytes[i] != 0 ) {return false;}
    }
    if (rem == 0){return true;}
    else {
        int i = idx;
        // e.g hex2bin() = 0001_1111 = 31
        return bytes[i] < pow(2, 8 - rem); 
    }

    return false;
} 

void copy_into_end(std::vector<uint8_t>& vec1,  std::vector<uint8_t>& vec2)
{
    std::vector<uint8_t>::iterator it1_start = vec1.begin() + vec1.size() - vec2.size();
    std::vector<uint8_t>::iterator it1;
    int idx {0};
    for (it1 = it1_start; it1 < it1_start + vec2.size(); it1++){
        *it1 = vec2[idx];
        idx += 1;
    }
}

unsigned long int Blockchain::proofOfWork(std::vector<uint8_t> bytes, unsigned long int target, unsigned long int start)
{
    /*
    * Simple proof of work
    * First n bits of the 256 bits of the SHA256 hash must be 0
    * Increasing the target by 1 doubles the difficulty and average time taken
    * Max difficulty = 256 (only valid solution is 256 zeros)
    * -- A standard pc has 2GHz ~= 2^30 calcs/s of processing power. 
    * -- Hashing power of this CPU is 470000 hashes/s ~= 2^18 hashes/s
    *    -> a target of 18 will take 1 seconds
    *    -> a target of 19 will take 2 seconds
    *    -> a target of 20 will take 4 seconds
    *    .... 
    *    -> a target of 30 will take 1024 seconds ~= 28 minutes
    *    ... 
    *    -> a target of 43 will take 1 year
    *    -> current bitcoin target is 76. This will take 9 billion years
    */
    unsigned long MAX_DIFFICULTY = 256 ;
    unsigned long first = bytes.size() - sizeof(start);

    std::vector<uint8_t> bytes_WIP;
    unsigned long n = target > MAX_DIFFICULTY ? MAX_DIFFICULTY : target; //
    unsigned long int nonce = start;
    std::vector<uint8_t> nonceBytes = utilities::long2byte(nonce);
    bytes_WIP.assign(bytes.begin(), bytes.end() );

    copy_into_end(bytes_WIP, nonceBytes);
    
    unsigned char hash[SHA256::DIGEST_SIZE];
    memset(hash,0,SHA256::DIGEST_SIZE);
    SHA256 ctx = SHA256();
    ctx.init();
    ctx.update(bytes_WIP.data(), bytes_WIP.size());
    ctx.final(hash);

    while (!this->is_valid_proof(hash, n)){
        bytes_add_1(bytes_WIP, first);

        memset(hash,0,SHA256::DIGEST_SIZE);
        ctx.init();
        ctx.update(bytes_WIP.data(), bytes_WIP.size());
        ctx.final(hash);
    }

    nonceBytes = std::vector<uint8_t>(bytes_WIP.begin() + first, bytes_WIP.end());
    nonce  = utilities::byte2long(nonceBytes);

    return nonce;
}

