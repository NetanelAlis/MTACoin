#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"

class Server
{
    ulong  m_DifficultyLimit;
    BLOCK_T m_LastBlockThatWasChecked;

    bool verifyProofOfWork() const;
    bool verifyProofOfWork(const BLOCK_T& i_SuggestedBlock);
    bool isNewBlockSuggested(const BLOCK_T& i_SuggestedBlock)
    {return (i_SuggestedBlock == g_BlockChainHead) || (m_LastBlockThatWasChecked == g_SuggestedBlock);}
    void appendNewBlock(BLOCK_T& i_NewBlockChinHead) const;
    void printBlockChainHead(const BLOCK_T& i_BlockChainHead) const;
    bool  checkSrcAndBlockHash(const BLOCK_T& i_SuggestedBlock, ulong i_CheckSum) const;

public:
    static bool s_ReadyToAppendBlock;
    void SetDifficultyLimit(ulong i_DifficultyLimit);
    void CreateGenesisBlock(BLOCK_T& i_Block,int i_Difficulty);
    static void* ServerFlow(void* argv);
    void ManageBlockChain();
};

#endif // SERVER_HPP