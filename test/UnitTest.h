/*
 *  Created on: 23 October, 2020
 *      Author: Lior Sinai
 * Description: Minimal custom unit test framework
 * 
 * Sources
 * https://accu.org/journals/overload/10/52/crickett_368/
 */

#ifndef UNIT_TESTER_H
#define UNIT_TESTER_H

#include <iostream>
#include <string>
#include <list>

namespace TestTypes{
    bool isTrue(bool condition, const char* code, const char* file, size_t line);
    bool isFalse(bool condition, const char* code, const char* file, size_t line);
    bool assert_test(bool condition, const char* code, const char* file, size_t line);
};

#define IS_TRUE(exp) TestTypes::isTrue(exp, #exp, __FILE__, __LINE__)
#define IS_FALSE(exp) TestTypes::isFalse(exp, #exp, __FILE__, __LINE__)


class UnitTest {
public:
    UnitTest(): name("unit test") {}
    UnitTest(const std::string& name_): name(name_) {}
    virtual bool run() = 0;
    virtual bool setUp(){return true;};
    virtual std::string get_name(){
        return this->name;
    };
private:
    std::string name;
};


class TestSuite {
  public:
    TestSuite(): name("test suite") {} 
    TestSuite(const std::string& name_): name(name_) {}
    void addTest(UnitTest* test); 
    void run();
    
  private:
    int pass_count{0}; 
    int fail_count{0};
    int completed_count{0};
    int nTests{0};

    std::string name{""};
    std::list<UnitTest*> tests_collection;
};



#endif //  UNIT_TESTER_H
