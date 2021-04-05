#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <stdint.h>
#include <vector>
#include <tuple>

// opensource
#include "sha256/sha256.h"
#include "sha256/sha256.cpp" //doesn't link if don't include this??

// local
#include "backend/Block.h"
#include "backend/Block.cpp"
#include "backend/Token.h"
#include "backend/Tokeniser.h"
#include "backend/Tokeniser.cpp"
#include "backend/utilities.h"
#include "backend/utilities.cpp"
#include "backend/MerkleTree.h"
#include "backend/MerkleTree.cpp"
#include "backend/Blockchain.h"
#include "backend/Blockchain.cpp"

#include "demos.h"
#include "demos.cpp"

#include "loadBlockchain.h"
#include "loadBlockchain.cpp"

#include "UnitTest.cpp"
#include "UtilitiesTests.h"
#include "UtilitiesTests.cpp"
#include "MerkleTreeTests.h"
#include "MerkleTreeTests.cpp"

void MerkleTree_tester();
void utilities_tester();

int main(int argc, char *argv[])
{   
    utilities_tester();
    MerkleTree_tester();

    std::cout << "Press any key to exit" << std::endl;
    std::cin.get();
    return 0;
}


void utilities_tester(){
    std::unique_ptr<TestSuite> testSuite = std::make_unique<TestSuite>("utilities");

    Long2BytesTest* t1 = new Long2BytesTest();
    HexTest* t2 = new HexTest();
    Hex2DecTest* t3 = new Hex2DecTest();
    BytesAdd1Test* t4 =  new BytesAdd1Test();
    
    testSuite->addTest(t1);
    testSuite->addTest(t2);
    testSuite->addTest(t3);
    testSuite->addTest(t4);

    testSuite->run();

    delete t1, t2, t3, t4;
}


void MerkleTree_tester()
{
    std::unique_ptr<TestSuite> testSuite = std::make_unique<TestSuite>("Merkle tree");

    EmptyTreeTest* t1  = new EmptyTreeTest();
    SingleNodeTest* t2 = new SingleNodeTest();
    DoubleNodeTest* t3 = new DoubleNodeTest();
    OddNodeTest* t4    = new OddNodeTest();
    MultiNodeOddTest* t5 = new MultiNodeOddTest();
    MultiNodeEvenTest* t6 = new MultiNodeEvenTest();

    testSuite->addTest(t1);
    testSuite->addTest(t2);
    testSuite->addTest(t3);
    testSuite->addTest(t4);
    testSuite->addTest(t5);
    testSuite->addTest(t6);

    testSuite->run();

    delete t1, t2, t3, t4, t5, t6;
}
