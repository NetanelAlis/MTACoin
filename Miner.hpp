#ifndef MINER_HPP
#define MINER_HPP

#include "Utils.hpp"
#include "Server.hpp"

class Miner
{
private:
    int m_BlockHeight = 0;
    int m_Timestamp = 0; 
    unsigned int m_Hash = 0;       
    unsigned int m_PrevHash = INITIAL_PREV_HASH;  
    int m_Difficulty = 0;    
    int m_Nonce = 0;
    int m_MinerID = 0;
    ulong m_DifficultyLimit = 0;
    bool m_BlockWasCheckedByTheServer = 0;
    void Mine();
    void readHeadBlock();
    void createMindrBlock();
    void createMinerBlock();
    void getHash();
    bool isValidHash();
    void suggestBlock();
    void printSuggestion() const;
    void applyForSuggestion();
    void dummyMinerFlow();

public:
    void SetMinerID(int i_Id) {m_MinerID = i_Id; }
    void SetfficultyLimit(ulong i_DifficultyLimit){m_DifficultyLimit = i_DifficultyLimit;}
    static void* MinerFlow(void* arg);
    void SetBlockWasCheckedByTheServer(bool i_BlockWasCheckedByTheServer){m_BlockWasCheckedByTheServer = i_BlockWasCheckedByTheServer;}
    
};

#endif // MINER_HPP