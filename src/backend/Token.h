/*
 *  Created on: 14 March 2021
 *      Author: Lior Sinai
 * Description: a token for inserting into a block into a blockchain
 */


#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <chrono>
#include <vector>
#include <tuple>
#include <iomanip>      // std::setfill, std::setw
#include <iostream>


struct Token {
  std::time_t timeStamp;
  std::string author;
  std::string userName;
  std::string fileName;
  std::string fileHash;
  std::vector<uint8_t> fileData;
} ;

#endif // TOKEN_H