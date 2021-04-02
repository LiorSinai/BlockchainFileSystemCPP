/*
 *  Created on: 28 March 2021
 *      Author: Lior Sinai
 * Description: General exception for the Blockchain Filesystem
 */

#ifndef BLOCKCHAIN_FILE_SYSTEM_EXCEPTION_H
#define BLOCKCHAIN_FILE_SYSTEM_EXCEPTION_H

#include <stdexcept>
#include <iostream>

 class BlockchainFileSystemException:public std::exception
{
public:
    BlockchainFileSystemException(const std::string m="Blockchain is invalid"):msg(m){}
    const char* what(){return msg.c_str();}
private:
    std::string msg;
};

#endif// BLOCKCHAIN_FILE_SYSTEM_EXCEPTION_H