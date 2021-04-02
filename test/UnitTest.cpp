#include "UnitTest.h"

//Test types
bool TestTypes::assert_test(bool condition, const char* code, const char* file, size_t line)
{   
    if (!condition){
        printf("...FAILED [%s] at line %d in file %s", code, line, file);
        std::cout<<std::endl;
        return false;
    }
    else{
        printf("...ok");
        std::cout<<std::endl;
        return true;
    }
}

bool TestTypes::isTrue(bool condition, const char* code, const char* file, size_t line)
{
    return assert_test(condition, code, file, line);
}
bool TestTypes::isFalse(bool condition, const char* code, const char* file, size_t line)
{
    return assert_test(!condition, code, file, line);
}

//Test Suite
void TestSuite::addTest(UnitTest* test){
    this->tests_collection.push_back(test);
}  

void TestSuite::run(){
std::cout <<"\n---------------\nRunning tests for " << this->name << "\n";
    std::list<UnitTest*>::iterator it;
    nTests = 0;
    pass_count = 0;
    fail_count = 0;
    completed_count = 0;
    for (it= this->tests_collection.begin(); it != this->tests_collection.end(); it ++){
        UnitTest* test = *it;
        ++nTests;
        bool passed;
        try{
            std::cout <<"Test " << test->get_name() << std::endl;
            if (test->setUp()) {
            try {
                passed = test->run();
                pass_count += passed;
                completed_count++;
            }
            catch(...) {
                printf("Error occurred while running test");
                std::cout<<std::endl;
                ++fail_count;
                continue;
            }
            }
            else {
                printf("setup failed");
                std::cout<<std::endl;
            }
        }
        catch (std::exception& e) {
            printf(e.what());
        }
        catch (...) {
            printf("Unexpected error");
        }
    }
    printf("%d of %d tests passed, %d aborted", pass_count, nTests, fail_count);
    std::cout<<std::endl;
}