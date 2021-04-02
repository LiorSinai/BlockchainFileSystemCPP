/*
 *  Created on: 27 March 2021
 *      Author: Lior Sinai
 * Description: Load blockchain from JSON
 */


#include "loadBlockchain.h"

using namespace minijson;

std::unique_ptr<Blockchain> loadBlockchainFromJSON (std::string fileName, std::string pathToDir)
{   
    std::cout << "loading blockchain ... ";

    std::unique_ptr<Blockchain> blockchain;
    std::string directory;
    std::time_t timeStamp;
    std::vector<PseudoBlock> pseudo_blocks;

    /* read file */    
    std::ifstream stream(fileName);
    if(stream.fail()){
        throw utilities::FileNotFoundException(fileName);
    }
    std::string json_str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    char* json_obj = &json_str[0];
    int nblocks {0};

    /* parse JSON */
    buffer_context ctx(json_obj, json_str.length());

    parse_object(ctx, [&](const char* k, value v)
    {
        dispatch (k)
        <<"directory">> [&]{ directory = v.as_string(); }
        <<"timeStamp">> [&]{ timeStamp = v.as_long(); }
        <<"blocks">> [&]
        {
            parse_array(ctx, [&](value v)
            {
                PseudoBlock b {};
                b.index = nblocks;
                parse_object(ctx, [&](const char* k, value v)
                {
                    dispatch (k)
                    <<"previousHash">>[&]{b.previousHash = v.as_string();}
                    <<"directory">> [&]{b.directory = v.as_string(); }
                    <<"timeStamp">> [&]{b.timeStamp = v.as_long(); }
                    <<"nonce">>  [&]{b.nonce = v.as_long(); }
                    <<"target">> [&]{b.target = v.as_long(); }
                    <<"tokens">> [&]
                    {
                        parse_array(ctx, [&](value v)
                        {   
                            Token token {};
                            std::string id{};
                            parse_object(ctx, [&](const char* k, value v)
                            {   
                                dispatch (k)
                                <<"id">>[&]{id = v.as_string();}
                                <<"token">>[&]
                                {
                                    parse_object(ctx, [&](const char* k, value v)
                                    {
                                        dispatch (k)
                                        <<"userName">> [&]{ token.userName = v.as_string(); }
                                        <<"fileName">> [&]{ token.fileName = v.as_string(); }
                                        <<"fileHash">> [&]{ token.fileHash = v.as_string(); }
                                        <<"author">> [&]{ token.author = v.as_string(); }
                                        <<"timeStamp">> [&]{ token.timeStamp = v.as_long(); }
                                         <<any>> [&]{ ignore(ctx); };
                                    }); // token
                                } 
                                <<any>> [&]{ ignore(ctx); };
                            }); // token map
                            b.tokens.insert(std::pair<std::string, Token>(id, token));
                        }); // tokens array
                    } 
                    <<any>> [&]{ ignore(ctx); };
                }); // block
                nblocks += 1;
                pseudo_blocks.push_back(b);
            }); //blocks array
        } 
        <<any>> [&]{ ignore(ctx); };
    });

    /* rebuild blockchain */
    blockchain = std::make_unique<Blockchain>(directory, timeStamp);
    blockchain->setDirectory(pathToDir + "/" + directory);
    for (auto b: pseudo_blocks){
        std::string errormsg = "ERROR: aborting at block " + std::to_string(b.index) + "\n";
        Block block = Block(b.index, b.previousHash, pathToDir + "/" + directory, b.timeStamp, b.target, b.nonce);
        std::map<std::string, Token>::iterator itr; 
        for (itr = b.tokens.begin(); itr != b.tokens.end(); ++itr) { 
            block.getTokens().insert(std::pair<std::string, Token>(itr->first, itr->second));
        }
        try {
            blockchain->commitBlock(block, false);
        }
        catch (InvalidBlockException &e){
            throw InvalidBlockchainException(errormsg + e.what());
        }
        catch (InvalidBlockchainException &e){
            throw InvalidBlockchainException(errormsg + e.what());
        }
        catch (const std::exception& e )
        {
            std::string msg = errormsg + e.what();
            throw std::exception(msg.c_str());
        }
    }

    std::cout << "done" << std::endl;
    /* verify */
    std::cout << "verifying blockchain ..." << std::endl;
    std::string errormsg = "ERROR: dropping blockchain\n" ;
    try{
        blockchain->verify();
        std::cout << "verified!" << std::endl;
    }
    catch (FileNotFoundException& e )
    {   
        throw FileNotFoundException(errormsg + e.what());
    }
    catch (InvalidBlockchainException &e){
       throw InvalidBlockchainException(errormsg + e.what());
    }
    catch (const std::exception& e )
    {
        throw InvalidBlockchainException(errormsg + e.what());
    }
    catch (...)
    {
        throw;
    }
    return blockchain;
}