#include "Server.hpp"

void Server::CreateGenesisBlock(BLOCK_T& i_Block) 
{
    i_Block = { 0, 0, 0, 0, 0, 0, 0 };
}

void* Server::ServerFlow(void* i_Server)
{
    Server* server = static_cast<Server*>(i_Server);

    server->ManageBlockChain();
}

void Server:: ManageBlockChain() 
{
    while(true)
    {
        pthread_mutex_lock(&g_SuggestedBlockLock);
        if(isNewBlockSuggested(g_SuggestedBlock))
        {
            pthread_cond_wait(&g_newSuggestedBlock,&g_SuggestedBlockLock);
        }
        if(verifyProofOfWork(g_SuggestedBlock))
        {
            appendNewBlock(g_SuggestedBlock);
        }
          
          pthread_mutex_unlock(&g_SuggestedBlockLock);
    }

}

bool Server:: verifyProofOfWork(const BLOCK_T& i_SuggestedBlock) 
{
    bool blockVerified = false;
    unsigned int crcParams[] = {i_SuggestedBlock.height,
    i_SuggestedBlock.timestamp,
    i_SuggestedBlock.prev_hash,
    i_SuggestedBlock.nonce
    ,i_SuggestedBlock.relayed_by};
    ulong checkSum;

    checkSum = crc32(i_SuggestedBlock.prev_hash,(const Bytef*)crcParams,sizeof(crcParams));
    if(checkSum == i_SuggestedBlock.hash)
    {
        if(i_SuggestedBlock.hash < this->m_DifficultyLimit)
        {
            blockVerified = true;
        }
    }
    
    return blockVerified;
}

void Server:: appendNewBlock(BLOCK_T& i_NewBlockChinHead) const
{
    g_BlockChainHead = i_NewBlockChinHead;
    printBlockChainHead(i_NewBlockChinHead);
}

void Server:: printBlockChainHead(const BLOCK_T& i_BlockChainHead) const
{
    std::cout << "server: New block added by " << g_BlockChainHead.relayed_by << ", attributes: ";
    std::cout << "height(" << g_BlockChainHead.height << ") ";
    std::cout << "timestamp(" << g_BlockChainHead.timestamp << ") ";
    std::cout << "hash(" << g_BlockChainHead.hash << ") ";
    std::cout << "prev_hash(" << g_BlockChainHead.prev_hash << ") ";
    std::cout << "difficulty(" << g_BlockChainHead.difficulty << ") ";
    std::cout << "nonce(" << g_BlockChainHead.nonce << ") \n";
}