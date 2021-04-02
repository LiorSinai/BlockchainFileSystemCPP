/*
 *  Created on: 15 March 2021
 *      Author: Lior Sinai
 * Description: prepare a file into a pseudo-token and then into a token
 */

#ifndef DEMOS_H
#define DEMOS_H

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

void demo_SHA256();
void demo_time();
void demo_bytes();
void demo_sha256_file();
void demo_tokeniser();
void demo_block();

#endif //DEMOS_H