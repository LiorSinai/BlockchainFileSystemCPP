/*
 *  Created on: 20 March 2021
 *      Author: Lior Sinai
 * Description: Tests for the Blockchain
 * Hashes tested at: https://emn178.github.io/online-tools/sha256.html
 */

#include "MerkleTreeTests.h"

bool EmptyTreeTest::run()
{
    MerkleTree tree = MerkleTree();
    std::vector<std::string> hashes;

    std::string result = tree.root(hashes);
    std::string expected = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";

    return IS_TRUE(result == expected);
}

bool SingleNodeTest::run()
{
    MerkleTree tree = MerkleTree();
    std::vector<std::string> hashes {"2a2da09e89fae0f94901eb737f5ed39dd835e4ee4be10fa1ead0416c78c29fa1"};

    std::string result = tree.root(hashes);
    std::string expected = hashes[0];
    return IS_TRUE(result == expected);
}

bool DoubleNodeTest::run()
{
    MerkleTree tree = MerkleTree();
    std::vector<std::string> hashes {
        "2a2da09e89fae0f94901eb737f5ed39dd835e4ee4be10fa1ead0416c78c29fa1",
        "85760bfd24bc3186e4f0bbcb95e87923c2ee1369b01103098390206d426b1681"
        };

    std::string result = tree.root(hashes);
    std::string expected = "ab26e069d5612422da10856057f8f4a47585e2ca6e528a3ed045fd306e89b60c";
    return IS_TRUE(result == expected);
}

bool OddNodeTest::run()
{
    MerkleTree tree = MerkleTree();
    std::vector<std::string> hashes {
        "2a2da09e89fae0f94901eb737f5ed39dd835e4ee4be10fa1ead0416c78c29fa1",
        "85760bfd24bc3186e4f0bbcb95e87923c2ee1369b01103098390206d426b1681",
        "db8f88342a392d907a51897ab5901827c3d29d0b9f942a47e143786d2f48a052"
        };

    //12->a: ab26e069d5612422da10856057f8f4a47585e2ca6e528a3ed045fd306e89b60c
    //33->b: 244dea23ed69a8d169a87bf22efeac2dd21b280282b0895a11d6d003fb1c5ff7
    //ab:    951b9ca47cc4c44d1ee822872163ba97a8935ff9b6b491b736cadf5eab9317e4

    std::string result = tree.root(hashes);
    std::string expected = "951b9ca47cc4c44d1ee822872163ba97a8935ff9b6b491b736cadf5eab9317e4";
    return IS_TRUE(result == expected);
}

bool MultiNodeOddTest::run()
{   
    MerkleTree tree = MerkleTree();
    std::vector<std::string> hashes {
        "eb810a4f75d23378039a6f24500fbd23a93b6675ccd4909a5ac0f431b8f6505c",
        "da201d5cef3f0baf42894e078d18a0d4f386d0513f6dbdd9e8d8a04799da98a7",
        "625da44e4eaf58d61cf048d168aa6f5e492dea166d8bb54ec06c30de07db57e1",
        "50009ce1da4d15e1c4a04024df691eed5f0d598e2c4c67092f205366d0adf99e",
        "6725e7bbcd28f3a8a586fa34bf191fd72dde8b61756932cd3237c17a6f196f1a"
        };

    //12->a: cf5f88cc77e2ea67abf4b94e0919d651341a81782b05da6199c9508c8969854d
    //34->b: 48084c43673ebddd8cfa5af8d2397e5259116d0e2f15069efde1b369d6c6786c
    //55->c: 4c928a262211d2409aa6e20ec36b091c2941595314f34b92bf10eb0d30bd73f8

    //ab->d: 6b403d7372a5f206c63aaeb50bb86c770c2412b691a3554cb8995f3da457d56d
    //cc->e: ee915427519e1b023632bdaba01eb6795b01251e643ae070ac49dc9ef8ae49c4
    
    //de:    f52ad28711e467b790e2128686a25163a44b003fd97247ca657cec6cebed675b
        
    std::string result = tree.root(hashes);
    std::string expected = "f52ad28711e467b790e2128686a25163a44b003fd97247ca657cec6cebed675b";
    return IS_TRUE(result == expected);
}


bool MultiNodeEvenTest::run()
{   
    MerkleTree tree = MerkleTree();
    std::vector<std::string> hashes {
        "eb810a4f75d23378039a6f24500fbd23a93b6675ccd4909a5ac0f431b8f6505c",
        "da201d5cef3f0baf42894e078d18a0d4f386d0513f6dbdd9e8d8a04799da98a7",
        "625da44e4eaf58d61cf048d168aa6f5e492dea166d8bb54ec06c30de07db57e1",
        "50009ce1da4d15e1c4a04024df691eed5f0d598e2c4c67092f205366d0adf99e",
        "6725e7bbcd28f3a8a586fa34bf191fd72dde8b61756932cd3237c17a6f196f1a",
        "a898be2fe1d1b8a3fdba26456f8ff107a30f99cfabd1d12568a0db6135ab180b"
        };

    //12->a: cf5f88cc77e2ea67abf4b94e0919d651341a81782b05da6199c9508c8969854d
    //34->b: 48084c43673ebddd8cfa5af8d2397e5259116d0e2f15069efde1b369d6c6786c
    //56->c: 6d90bd6f949975c81c8e9a30decbc62fe69b56e65d54c913037175fec288599a

    //ab->d: 6b403d7372a5f206c63aaeb50bb86c770c2412b691a3554cb8995f3da457d56d
    //cc->e: 55d2f5822fb693953357d78373f7c64f5f2d8cee4aceef9319eca49191de39f6
    
    //de:    eaa7a488c90b2362af77298baf3faca9407a5eecd0805b5c0183dbb1e58e9786
        
    std::string result = tree.root(hashes);
    std::string expected = "eaa7a488c90b2362af77298baf3faca9407a5eecd0805b5c0183dbb1e58e9786";
    return IS_TRUE(result == expected);
}



