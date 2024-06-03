#ifndef MINER_HPP
#define MINER_HPP

#include "Utils.hpp"
#include "Server.hpp"

class Miner
{
private:
    int m_BlockHeight;
    int m_Timestamp; 
    unsigned int m_Hash;       
    unsigned int m_PrevHash = INITIAL_PREV_HASH;  
    int m_Difficulty;    
    int m_Nonce;          
    int m_MinerID;
    ulong m_DifficultyLimit;
    bool m_BlockWasCheckedByTheServer;

    void Mine();
    void readHeadBlock();
    void createMindrBlock();
    void createMinerBlock();
    void getHash();
    bool isValidHash();
    void suggestBlock();
    void printSuggestion() const;

public:
    void SetMinerID(int i_Id) {m_MinerID = i_Id; }
    void SetfficultyLimit(ulong i_DifficultyLimit){m_DifficultyLimit = i_DifficultyLimit;}
    static void* MinerFlow(void* arg);
    void SetBlockWasCheckedByTheServer(bool i_BlockWasCheckedByTheServer){m_BlockWasCheckedByTheServer = i_BlockWasCheckedByTheServer;}
};

#endif // MINER_HPP