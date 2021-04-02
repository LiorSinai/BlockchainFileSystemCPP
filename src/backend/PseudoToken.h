/*
 *  Created on: 14 March 2021
 *      Author: Lior Sinai
 * Description: a pseudo token for making a real token in the blockchain. Goes on waiting list.
 */


#ifndef PSEUDO_TOKEN_H
#define PSEUDO_TOKEN_H

#include <string>
#include <chrono>
#include <vector>
#include <tuple>


struct PseudoToken {
  std::string userName;
  std::string filePath;
  std::string author;
} ;

#endif // PSEUDO_TOKEN_H