/*
 *  Created on: 15 March 2021
 *      Author: Lior Sinai
 * Description: prepare a pseudo-token for entering into a block
 */

#include "Tokeniser.h"

Token Tokeniser::make_token(PseudoToken pseudoToken)
{
    //extract strings from pseudo-token
    std::string userName = pseudoToken.userName;
    std::string author = pseudoToken.author;

    std::string filePath = pseudoToken.filePath;

    std::size_t found   = filePath.find_last_of("/\\");
    if (found == std::string::npos) found = -1;
    std::string fileName = filePath.substr(found+1);
    
    // time
    std::time_t creation_time = std::time(0);

    // get file data
    std::vector<uint8_t> fileData;
    long fileSize = utilities::getFileSize(filePath);
    if (fileSize < MAX_FILE_SIZE)
    {   
        bool loaded = utilities::loadFile(filePath, fileData);
        if (!loaded){
            throw utilities::FileNotFoundException(filePath);
        }

        std::string outPath (this->directory + fileName);
        bool copied = utilities::copyFile(filePath, outPath);
        if (!copied){
            std::string msg =  "WARNING: copying file failed. Please create directory " + this->directory + " if it does not exist.\n";
            throw std::exception(msg.c_str());
        }
    }
    else {
        std::string msg = "WARNING: file size exceed maximum file size, " + std::to_string(fileSize/1e6)+ "MB>" + std::to_string(MAX_FILE_SIZE/1e6) +"MB" ;
        throw std::exception(msg.c_str());
    }
    std::string fileHash = sha256(fileData.data(), fileData.size());

    // load into Token
    Token token {};
    token.userName = userName;
    token.author   = author;
    token.fileName = fileName;
    //token.fileData = fileData; //store this external to the blockchain metadata
    token.fileHash = fileHash;
    token.timeStamp = creation_time;

    return token;
}

void Tokeniser::print_token(Token token){
    long max_show_file = 50;
    int show_start = 20;
    int show_end   = 20;

    std::string outstring;
    outstring.append("User name: " + token.userName + "\n");
    outstring.append("file name: " + token.fileName + "\n");
    outstring.append("file hash: " + token.fileHash + "\n");
    
    //temporarily load file data
    std::vector<uint8_t> fileData;
    std::string filePath (this->directory + token.fileName);
    std::stringstream  fileDataStream;
    bool loaded = utilities::loadFile(filePath, fileData);
    long fileSize = fileData.size();
    if (!loaded){
        fileDataStream << "WARNING: no file loaded from " << filePath ;
    }
    else{
        // possibly shorten output data
        fileDataStream << "0x";
        if (fileSize > max_show_file){
            for (int i{0}; i < show_start; i++){
                fileDataStream << std::setfill('0') << std::hex << std::setw(2) << (int)fileData[i];
            }
            fileDataStream << "..." ;
            for (int i{fileSize-show_end}; i < (fileSize); i++){
                fileDataStream << std::setfill('0') << std::hex << std::setw(2) << (int)fileData[i];
            }
        }
        else{
            for (int i{0}; i < fileSize; i++){
                fileDataStream << std::setfill('0') << std::hex << std::setw(2) << (int)fileData[i];
            }
        }

    }

    outstring.append("file size: ");
    if (fileSize > 1000000000){
        outstring.append(std::to_string(fileSize/1000000000) + "GB\n");
    }
    else if (fileSize > 1000000){
        outstring.append(std::to_string(fileSize/1000000) + "MB\n");
    }
    else if (fileSize > 1000){
        outstring.append(std::to_string(fileSize/1000) + "kB\n");
    }
    else {
        outstring.append(std::to_string(fileSize) + "B\n");
    }
    outstring.append("file data: " + fileDataStream.str() + "\n");

    outstring.append("file author: " + token.author + "\n");
    tm* gmtm = gmtime(&token.timeStamp);
    std::string strTime = std::asctime(gmtm);
    outstring.append("token creation time UTC: " + strTime);

    std::cout << outstring << std::endl;
}


std::vector<uint8_t> Tokeniser::serialise(Token token)
{
    std::vector<uint8_t> header;
    for (char c: (token.userName + token.fileName)){
        header.push_back(static_cast<uint8_t>(c));
    }
    std::vector<uint8_t>hashbytes;
    try{
        hashbytes = utilities::hex2byte(token.fileHash);
    }
    catch (std::exception &e)
    {
        throw;
    }
    header.insert(header.end(), hashbytes.begin(), hashbytes.end());
    for (char c: token.author){
        header.push_back(static_cast<uint8_t>(c));
    }
    std::vector<uint8_t> timebytes = utilities::long2byte(token.timeStamp);
    header.insert(header.end(), timebytes.begin(), timebytes.end());

    return header;
}

std::string Tokeniser::token_to_json(Token token, int level)
{
    std::unique_ptr<JSONwriter> writer = std::make_unique<JSONwriter>(level);
    writer->write("userName", token.userName);
    writer->write("fileName", token.fileName);
    writer->write("fileHash", token.fileHash);
    writer->write("author", token.author);
    writer->write("timeStamp", (long) token.timeStamp);
    return writer->result();
}

std::string Tokeniser::hash_token(Token token)
{
    std::vector<uint8_t> header = serialise(token);
    return sha256(header.data(), header.size());
}


bool Tokeniser::verify_token(Token token){
    /* load token data and check hash */
    std::vector<uint8_t> fileData;
    std::string filePath (this->directory + token.fileName);
    bool loaded = utilities::loadFile(filePath, fileData);
    if (!loaded){
        throw utilities::FileNotFoundException(filePath);
    }
    std::string fileHash = sha256(fileData.data(), fileData.size());
    if (fileHash != token.fileHash){
        std::string msg =  token.fileName + std::string(" hashes do not match:\nOriginal:   ") + token.fileHash + std::string("\nCalculated: ") + fileHash;
        throw InvalidTokenException(msg);
    }

    return true;
}