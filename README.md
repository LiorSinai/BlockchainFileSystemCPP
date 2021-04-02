# BlockchainFileSystemCpp

A blockchain file system. This program moves files into a directory which is registered on a private blockchain.
The blockchain meta data is stored in JSON format in the directory.
This blockchain is "immutable". 
That is, any change to any file registered inside a block will make that block and all subsequent blocks after it invalid.

This project is essentially a very basic version control system.
Hence the terminology used is based on version control software, such "stage" and "commit" instead of crypto-currency terminology such as "mine" and  "confirm". 
Ironically it itself is versioned and hosted GitHub using Git, a much more powerful version control software. 
This project however is not meant to be a production ready system; it is simply to make the concept of a blockchain more tangible for teaching purposes.

## Blockchain Features

1. Immutable: any change to any file registered inside a block will make that block and all subsequent blocks after it invalid
2. <s>De</s>Centralized
	1. <s>Consenesus mechanism </s>
	2. <s>Propagation mechanism </s>
	2. <s>Central Waiting list </s>
3. Proof of work
	1. Manual setting of difficulty from level 0 to 256. Default is 0.
	2. <s> Automatic difficulty adjustment mechanism </s>
4. Storage: the blockchain can be saved in JSON format and reloaded from JSON format. Files are stored in a standard OS directory.

It was a design choice to store the files in a normal directory, instead of lumping them into a single binary file.
This makes the blockchain more tangible and interactive.
However, it is STRONGLY ADVISEABLE TO NOT CHANGE ANYTHING in the blockchain directory because this will probably set the blockchain into an invalid state.
The exception is to make new folders when requested.

## Command Line Interface
After building the C++ project, run `main.exe`:

### Commands
Available commands:
```
 -c | --commit-block             commit a block to the blockchain 
      --demo                     run a demo
 -h | --help                     show this help menu
      --header [index]           show the raw header for a block
 -i | --initialise               initialise a new blockchain
 -l | --load filePath [dirPath]  load the blockchain from a .json file (include extension).
 -p | --print-blockchain         print the blockchain
      --print-block [index]      print block at index in the blockchain.
      --print-token [id]         print token with hash=id.
 -q | --quit                     quit
 -s | --save                     save the blockchain to a file
      --sha256 text              calculate the sha256 hash of text (rest of the line)
      --sha256-hex hexnum        calculate the sha256 hash of a hexadecimal number
      --sha256-file filepath     calculate the sha256 hash of the file
      --switch-user username     change the active user
 -t | --stage-token              stage a token into a block
      --target [target]          set the proof of work target (0-256). Display the target if no option 
 -u | --unstage                  delete the last uncommitted block
      --verify                   verify the blockchain (excludes uncommitted blocks)
 -v | --version                  print this version
 ```
 
 ### Examples
 #### Initialise
  In a terminal, navigate to src and run ```main```. When prompted for a username, enter a name or push enter for the default (@admin).
 Then run the following:
 ```
 @admin > --initialise   //follow prompts to initialise a new block chain
 @admin > --stage-token  //follow prompts to stage a new token
 @admin > --print-block  
 @admin > --commit-block  
 @admin > --print-blockchain
 ```
 
 #### Load
 In a terminal, navigate to src and run ```main```. When prompted for a username, enter a name or push enter for the default (@admin).
 Then run the following:
 ```
 @admin > --demo
 //... final line is:
 //writing to file ../MyBlockchain/MyBlockchain.json ... done
 @admin > --load ../MyBlockchain/MyBlockchain.json .. //two dots are important to show the folder is in the parent directory
 //... final line is:
 //verified!
 @admin > --stage-token   //follow prompts to stage a new token
 @admin > --print-block  
 @admin > --commit-block  
 @admin > --save 
 ```
 
 
## Proof of work

Proof of work is not necessary for a centralised system. 
It is included here for completeness.

The proof of work is identical to the Bitcoin proof of work.
That is, the SHA256 of the block header must be less than ```2^(256-n)``` for a target ```n```.
Equivalent ways of stating this are: the 256 bit hash must start with ```n``` zeros, or the 64 character hexadecimal hash must start with ```n/4``` zeros.
Unlike Bitcoin, ```n``` cannot have decimals, so it is an integer from ```0``` to ```256```.

Increasing the target by 1 doubles the difficulty and average time taken.
A standard pc has 2GHz ~= 2^30 calcs/s of processing power.
For my CPU, this means it can do about 120,000 hashes/s ~= 2^16 hashes/s.
This means:
* a target of 16 takes 1 seconds
* a target of 17 takes 2 seconds
* a target of 18 takes 4 seconds
*    .... 
* a target of 20 takes 16 seconds
* a target of 30 will take 16384 seconds ~= 4.5 hours
* a target of 50 will take 1 year
* a target of 76 will take 36 billion years

A target of 76 was the Bitcoin difficulty at the time of this commit. (Conversion: 256 - 208 - log2((2^16-1)/D) where D is the difficulty.)
It is strongly recommended to keep the target at 0.

## To do list:
- Automatic folder creation using Boost's [Filesystem library](https://www.boost.org/doc/libs/1_75_0/libs/filesystem/doc/index.htm). 
This is not platform independent which makes it tricky to do in C++.
- Incremental saving to JSON. The current code saves the whole blockchain each time.
- Threading, so the program can send messages back to the CLI for outputting interval messages during the proof of work. 

## Dependences
- [`MiniJson Reader https://github.com/giacomodrago/minijson_reader`](https://github.com/giacomodrago/minijson_reader)
- ['C++ sha256 function http://www.zedwood.com/article/cpp-sha256-function'](http://www.zedwood.com/article/cpp-sha256-function). 

