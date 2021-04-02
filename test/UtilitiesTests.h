/*
 *  Created on: 20 March 2021
 *      Author: Lior Sinai
 * Description: Tests for the utility byte to hexidecimal/decimal conversions
 *
 */

#ifndef UTILITIES_TESTS_H
#define UTILITIES_TESTS_H

#include "UnitTest.h"
#include "backend/utilities.h"

class Long2BytesTest: public UnitTest{
public:
    Long2BytesTest() : UnitTest("Long2BytesTest") {};
    bool run();
};

class HexTest: public UnitTest{
public:
    HexTest() : UnitTest("HexTest") {};
    bool run();
};

class Hex2DecTest: public UnitTest{
public:
    Hex2DecTest() : UnitTest("Hex2DecTest") {};
    bool run();
};

#endif //UTILITIES_TESTS_H