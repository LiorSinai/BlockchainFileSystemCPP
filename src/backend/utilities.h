/*
 *  Created on: 14 March 2021
 *      Author: Lior Sinai
 * Description: Utilities for the blockchain
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <string> 
#include <iostream>
#include <fstream>   
#include <sstream>   
#include <vector>
#include <tuple>

namespace utilities{
inline bool ends_with(std::string const & value, std::string const & ending);

long getFileSize(std::string const &fileName);


class FileNotFoundException:public std::exception
{
public:
    FileNotFoundException(const std::string filename=""){this->msg = "File not found: " + filename;}
    const char* what(){return msg.c_str();}
private:
    std::string msg;
};



bool loadFile(std::string const & fileName, std::vector<uint8_t> &contents);
bool loadFile(std::string const & fileName, std::vector<char> &contents);
void writeTextFile(std::string const & fileName, std::vector<uint8_t> &contents);
bool copyFile(std::string const & sourcePath, std::string const & destPath);

std::string hex2bin(std::string hexdec);
uint8_t hex2unit8 (char hexdec);
std::vector<uint8_t> hex2byte(std::string hexdec);
std::string byte2hex(std::vector<uint8_t> &bytes);
unsigned long long hex2dec(std::string hex);

std::vector<uint8_t> long2byte(unsigned long x);
std::vector<uint8_t> longlong2byte(unsigned long long x);
unsigned long  byte2long(std::vector<uint8_t>& bytes);
unsigned long long  byte2longlong(std::vector<uint8_t>& bytes);

void bytes_add_1(std::vector<uint8_t>& bytes, int first = 0);

}

#endif // UTILITIES_H