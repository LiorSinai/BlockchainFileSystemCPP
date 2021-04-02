/*
 *  Created on: 20 March 2021
 *      Author: Lior Sinai
 * Description: Tests for the Blockchain
 * 
 */

#include "UtilitiesTests.h"

using namespace utilities;

bool Long2BytesTest::run()
{
    bool passed = true;
    std::vector<uint8_t> bytes ;
    std::vector<uint8_t> result;
    long x;
    
    x = 5 + (6 * 256) + (7 * 256 * 256) + (11 * 256 * 256 * 256);
    result = long2byte(x);
    bytes = {11, 7, 6, 5};
    passed = passed && IS_TRUE(result == bytes);

    long y = byte2long(bytes);
    passed = passed && IS_TRUE(y == x);    

    return passed;
}

bool HexTest::run()
{   
    bool passed = true;
    std::vector<uint8_t> bytes ;
    std::vector<uint8_t> result;

    std::string hex {"7b749d3e23da4867abf9e541608e104f898628f8e213fa901a35cf997d5c9a18"};
    bytes = {
        123, 116, 157, 62, 35, 218, 72, 103, 171, 249, 229, 65, 96, 142, 16, 79, 137, 134, 40, 248, 226, 19, 
        250, 144, 26, 53, 207, 153, 125, 92, 154, 24
    };
    result = hex2byte(hex);
    passed = passed && IS_TRUE(result == bytes);   

    std::string result2 = byte2hex(bytes);

    passed = passed && IS_TRUE(result2 == hex);   
    return passed;
}

bool Hex2DecTest::run()
{   
    bool passed = true;
    unsigned long long x = 2 * pow(2, 63);

    std::string hex1 = byte2hex(long2byte(x)); // truncates the conversion
    unsigned long long y1 = hex2dec(hex1);
    passed = passed && IS_FALSE(x == y1);  

    std::string hex2 = byte2hex(longlong2byte(x)); 
    unsigned long long y2 = hex2dec(hex2);
    passed = passed && IS_TRUE(x == y2);  

    return passed;
}
