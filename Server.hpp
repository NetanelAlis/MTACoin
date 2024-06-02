#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"

class Server
{
    u_long m_DifficultyLimit;
    BLOCK_T m_LastBlockThatWasChecked;

    bool verifyProofOfWork() const;
    bool verifyProofOfWork(const BLOCK_T& i_SuggestedBlock);
    bool isNewBlockSuggested(const BLOCK_T& i_SuggestedBlock)
    {return (i_SuggestedBlock == g_BlockChainHead) || (m_LastBlockThatWasChecked == g_SuggestedBlock);}
    void appendNewBlock(BLOCK_T& i_NewBlockChinHead) const;
    void printBlockChainHead(const BLOCK_T& i_BlockChainHead) const;



public:
    void CreateGenesisBlock(BLOCK_T& i_Block);
    static void* ServerFlow(void* argv);
    void ManageBlockChain();
    Server()
    {
        m_DifficultyLimit = pow((float)2,(float)(32-g_BlockChainHead.difficulty));
        CreateGenesisBlock(m_LastBlockThatWasChecked);
    }
};

#endif // SERVER_HPP