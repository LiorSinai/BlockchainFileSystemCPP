/*
 *  Created on: 27 March 2021
 *      Author: Lior Sinai
 * Description: JSON writer. 
 *      Source: https://www.json.org/json-en.html
 */

#ifndef JSON_WRITER_H
#define JSON_WRITER_H

#include <sstream> 
#include <string>
#include <vector>
#include <map>

enum class JSONvalueType{ JSON_string, JSON_number, JSON_null, JSON_bool, JSON_object};

class JSONwriter 
{
public:
    JSONwriter(int level_);

    void write(JSONvalueType T, std::string fieldname, std::string fieldvalue);
    void write(std::string fieldname, std::string fieldvalue);
    void write(std::string fieldname, int fieldvalue);
    void write(std::string fieldname, long fieldvalue);
    void write(std::string fieldname, unsigned long fieldvalue);
    void write(std::string fieldname, bool fieldvalue);
    void write(std::string fieldname);

    template <typename valType>
    void write_array(std::string fieldname, std::vector<valType> arr);
    template <typename  keyType, typename  valType> 
    void write_map(std::string fieldname, std::map<keyType, valType> arr, std::string keyName, std::string elementName);

    void write_object(std::string fieldname, std::string fieldvalue);

    void clear();

    std::string result();
private:
    int level;
    bool empty {true};
    std::stringstream stream;
    std::string delim_outer;
    std::string delim_inner;
    void write_comma();
};

JSONwriter::JSONwriter(int level_)
{
    this->level = level_;

    std::string tabs = "";
    for (int i{0}; i<level; i++){
        tabs += "\t";
    }
    delim_outer = "\n" + tabs;
    delim_inner = "\n" + tabs + "\t";
    this->stream << delim_outer << "{";
}

std::string JSONwriter::result()
{
    this-> stream << delim_outer << "}"; 
    return this->stream.str();
}

void::JSONwriter::write_comma()
{
    if (empty) {
        this->empty = false;
    }
    else {
        this->stream << ",";
    }
    this->stream << delim_inner;
}

template <typename valType>
void JSONwriter::write_array(std::string fieldname, std::vector<valType> arr)
{
    this->write_comma();
    bool first = true;
    this->stream << "\"" << fieldname << "\":[" ; 
    for (valType element: arr){
        if (first){
            first = false;
        }
        else{
            this->stream << ",";
            this->stream << delim_inner;
        }
        this->stream << "\t" << element; //assume object type
    }
    this->stream << delim_inner << "]"; 
}

template <typename  keyType, typename  valType> 
void JSONwriter::write_map(std::string fieldname, std::map<keyType, valType> arr, std::string keyName, std::string elementName)
{
    this->write_comma();
    bool first = true;
    this->stream << "\"" << fieldname << "\":[" ; 
    std::map<keyType, valType>::iterator itr; 
    std::string delim_outer_ = this->delim_outer + "\t\t";
    std::string delim_inner_ = this->delim_inner + "\t\t";

    for (itr = arr.begin(); itr != arr.end(); ++itr){
        if (first){
            first = false;
        }
        else{
            this->stream << ",";
        }
        this->stream << delim_outer_ << "{";
        this->stream << delim_inner_ << "\"" << keyName << "\":"  "\"" << itr->first << "\"," ;
        this->stream << delim_inner_ << "\"" << elementName << "\":" ;
        this->stream << itr->second; //assume object type
        this->stream << delim_outer_ << "}";
    }
    this->stream << delim_inner << "]"; 
}



void JSONwriter::write(JSONvalueType T, std::string fieldname, std::string fieldvalue)
{
    this->write_comma();
    this->stream << "\"" << fieldname << "\":" ; 
    if (T == JSONvalueType::JSON_string)
    {
        this->stream << "\"" << fieldvalue << "\"";
    }
    else if (T == JSONvalueType::JSON_number || T==JSONvalueType::JSON_bool || T==JSONvalueType::JSON_object){
        this->stream << fieldvalue;
    }
    else if (T == JSONvalueType::JSON_null){
        this->stream << "null";
    }
    else {
        throw std::invalid_argument("Invalid JSON type");
    }
}

void JSONwriter::write(std::string fieldname, std::string fieldvalue)
{
    this->write_comma();
    this->stream << "\"" << fieldname << "\":" ; 
    this->stream << "\"" << fieldvalue << "\"";
}

void JSONwriter::write(std::string fieldname, int fieldvalue)
{
    this->write_comma();
    this->stream << "\"" << fieldname << "\":" ; 
    this->stream << fieldvalue;
}

void JSONwriter::write(std::string fieldname, long fieldvalue)
{
    this->write_comma();
    this->stream << "\"" << fieldname << "\":" ; 
    this->stream << fieldvalue;
}

void JSONwriter::write(std::string fieldname, unsigned long fieldvalue)
{
    this->write_comma();
    this->stream << "\"" << fieldname << "\":" ; 
    this->stream << fieldvalue;
}


void JSONwriter::write(std::string fieldname, bool fieldvalue)
{
    this->write_comma();
    this->stream << "\"" << fieldname << "\":" ; 
    if (fieldvalue){
        this->stream << "true";
    }
    else{
        this->stream << "false";
    }
}


void JSONwriter::write(std::string fieldname)
{
    this->write_comma();
    this->stream << "\"" << fieldname << "\":" ; 
    this->stream << "null";
}

#endif //#define JSON_WRITER_H