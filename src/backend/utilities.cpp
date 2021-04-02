/*
 *  Created on: 14 March 2021
 *      Author: Lior Sinai
 * Description: Utilities for the blockchain
 */

#include "utilities.h"

namespace utilities{
    
inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

long getFileSize(std::string const &  fileName)
{
    std::ifstream stream(fileName.c_str());
    if(stream.fail()){
        return 0; //... maybe throw error here?
    }
    long begin, end;
    begin = stream.tellg();
    stream.seekg (0, std::ios::end);
    end = stream.tellg();

    stream.close();
    return end-begin;
}

bool loadFile(std::string const & fileName , std::vector<uint8_t> &contents)
{
    std::ifstream stream(fileName, std::ios::binary);
    if(stream.fail()){
        return false;
    }
    contents.insert(contents.begin(), std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    stream.close();
    return true;
}


void writeTextFile(std::string const & fileName, std::vector<uint8_t> &contents)
{
    std::ofstream outFile;
    outFile.open(fileName);
    if(!outFile)
    {
        std::cout<<"Writing to file failed"<<std::endl;
        return;
    }
    for (auto c: contents){
        outFile << c ;
    }

    outFile.close();
}

bool copyFile(std::string const & sourcePath, std::string const & destPath)
{
    std::ifstream src(sourcePath, std::ios::binary);
    std::ofstream dest(destPath, std::ios::binary);
    dest << src.rdbuf();
    return src && dest;
}


std::string hex2bin(std::string hexdec)
{   
    //https://www.geeksforgeeks.org/program-to-convert-hexadecimal-number-to-binary/
    int i = 0;
    std::stringstream ss;
    if ((hexdec[0]== '0')&& (hexdec[1]=='x')) i = 2;

    while (hexdec[i]) {
        switch (hexdec[i]) {
        case '0':
            ss << "0000";
            break;
        case '1':
            ss << "0001";
            break;
        case '2':
            ss << "0010";
            break;
        case '3':
            ss << "0011";
            break;
        case '4':
            ss << "0100";
            break;
        case '5':
            ss << "0101";
            break;
        case '6':
            ss << "0110";
            break;
        case '7':
            ss << "0111";
            break;
        case '8':
            ss << "1000";
            break;
        case '9':
            ss << "1001";
            break;
        case 'A':
        case 'a':
            ss << "1010";
            break;
        case 'B':
        case 'b':
            ss << "1011";
            break;
        case 'C':
        case 'c':
            ss << "1100";
            break;
        case 'D':
        case 'd':
            ss << "1101";
            break;
        case 'E':
        case 'e':
            ss << "1110";
            break;
        case 'F':
        case 'f':
            ss << "1111";
            break;
        default:
            ss << "\nInvalid hexadecimal digit " << hexdec[i] << ". Stopping conversion" << std::endl;
            return ss.str();
        }
        i++;
    }

    return ss.str();
}


uint8_t utilities::hex2unit8(char hexdec)
{
    uint8_t c; 

    switch (hexdec) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        c = (uint8_t) hexdec - '0'; 
        break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
        c = (uint8_t) hexdec - 'a' + 10; 
        break;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
        c = (uint8_t) hexdec - 'A' + 10; 
        break;
    default:
        throw std::invalid_argument(std::string("Unknown hexadecimal character: ") + hexdec);
    }
    return c;
}

std::vector<uint8_t> utilities::hex2byte(std::string hexdec)
{
    int i = 0;
    std::vector<uint8_t> vec;
    if (hexdec.size() % 2 != 0){
        hexdec = "0" + hexdec;
    }
    vec.reserve(hexdec.size()/2);
    if ((hexdec[0]== '0') && (hexdec[1]=='x')) i = 2;
    while (i < hexdec.size()) {
        vec.push_back((uint8_t)(hex2unit8(hexdec[i]) * 16 + hex2unit8(hexdec[i+1])));
        i += 2;
    }
    
    return vec;
}


std::string uint82hex(uint8_t d){
    if (0 <= d && d < 10){
        return std::to_string(d);
    }
    else if (d < 16){
        return std::string(1, (char) d + 87);
    }
    else{
        throw std::invalid_argument(std::string("Unknown hexadecimal size: ") + std::to_string(d));
    }
}

std::string byte2hex(std::vector<uint8_t> &bytes)
{
    std::string hexdec = "";
    int c1;
    int c2;

    for (uint8_t b: bytes){
        c1 = b/16;
        c2 = b - (16 * c1);
        hexdec += uint82hex(c1);
        hexdec += uint82hex(c2);
    }
    return hexdec;
}

std::vector<uint8_t> utilities::long2byte(unsigned long x)
{
    std::vector<uint8_t> v;
    v.reserve(sizeof(x));
    for (size_t i = 0; i < sizeof(x); ++i) {
        v.push_back((uint8_t) (x & 0xFF));
        x >>= 8;
    }
    std::reverse(v.begin(),v.end()); //make big endian
    return v;
}


std::vector<uint8_t> longlong2byte(unsigned long long x)
{
    std::vector<uint8_t> v;
    v.reserve(sizeof(x));
    for (size_t i = 0; i < sizeof(x); ++i) {
        v.push_back((uint8_t) (x & 0xFF));
        x >>= 8;
    }
    std::reverse(v.begin(),v.end()); //make big endian
    return v;
}


unsigned long utilities::byte2long(std::vector<uint8_t>& bytes)
{   
    unsigned long  x{0};
    unsigned long  base {1};

    for (std::vector<uint8_t>::reverse_iterator itr = bytes.rbegin();  itr != bytes.rend(); ++itr ) {
        x += (*itr) * base;
        base *= 256;
    }
    return x;
}

unsigned long long  utilities::byte2longlong(std::vector<uint8_t>& bytes)
{   
    unsigned long long  x{0};
    unsigned long long  base {1};

    for (std::vector<uint8_t>::reverse_iterator itr = bytes.rbegin();  itr != bytes.rend(); ++itr ) {
        x += (*itr) * base;
        base *= 256;
    }
    return x;
}

unsigned long long hex2dec(std::string hex){
    unsigned long long  x{0};
    unsigned long long  base {1};
    int end = ((hex[0]== '0') && (hex[1]=='x')) ? 2 : 0 ;

    for (int i = hex.length()-1 ; i>=end ; i--){
        x += hex2unit8(hex[i]) * base;
        base *= 16;
    }

    return x;
}
}
