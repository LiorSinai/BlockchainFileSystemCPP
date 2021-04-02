/*
 *  Created on: 20 March 2021
 *      Author: Lior Sinai
 * Description: Tests for the Merkle Tree
 *
 */

#ifndef MERKLE_TREE_TESTS_H
#define MERKLE_TREE_TESTS_H

#include "UnitTest.h"
#include "backend/MerkleTree.h"
#include "sha256/sha256.h"

class EmptyTreeTest: public UnitTest{
public:
    EmptyTreeTest() : UnitTest("EmptyTreeTest") {};
    bool run();
};

class SingleNodeTest: public UnitTest{
public:
    SingleNodeTest() : UnitTest("SingleNodeTest") {};
    bool run();
};

class DoubleNodeTest: public UnitTest{
public:
    DoubleNodeTest() : UnitTest("DoubleNodeTest") {};
    bool run();
};

class OddNodeTest: public UnitTest{
public:
    OddNodeTest() : UnitTest("OddNodeTest") {};
    bool run();
};


class MultiNodeOddTest: public UnitTest{
public:
    MultiNodeOddTest() : UnitTest("MultiNodeOddTest") {};
    bool run();
};

class MultiNodeEvenTest: public UnitTest{
public:
    MultiNodeEvenTest() : UnitTest("MultiNodeEvenTest") {};
    bool run();
};


#endif //MERKLE_TREE_TESTS_H