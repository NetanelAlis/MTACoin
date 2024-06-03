#include "Server.hpp"

bool Server::s_ReadyToAppendBlock = false;

void Server::SetDifficultyLimit(ulong i_DifficultyLimit)
{
    m_DifficultyLimit = i_DifficultyLimit;
}

void Server::CreateGenesisBlock(BLOCK_T& i_Block, int i_Difficulty)
{
    i_Block = { 0, 0, 0, 0, i_Difficulty, 0, 0 };
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
        
        try
        {
            verifyProofOfWork(g_SuggestedBlock);
            appendNewBlock(g_SuggestedBlock);
        }
        catch(const Exception& error)
        {
            error.Print();
        }

          g_BlockNeedToBeChecked = false;
          pthread_mutex_unlock(&g_SuggestedBlockLock);
          pthread_cond_broadcast(&g_newSuggestedBlock);
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

    m_LastBlockThatWasChecked = i_SuggestedBlock;
    checkSum = crc32(i_SuggestedBlock.prev_hash,(const Bytef*)crcParams,sizeof(crcParams));
    blockVerified = checkSrcAndBlockHash(i_SuggestedBlock,checkSum);

    return blockVerified;
}

bool Server:: checkSrcAndBlockHash(const BLOCK_T& i_SuggestedBlock, ulong i_CheckSum) const
{
    bool verified = false;

    if(i_SuggestedBlock.prev_hash == g_BlockChainHead.hash)
    {
        if(i_SuggestedBlock.height - 1 == g_BlockChainHead.height)
        {
            if(i_CheckSum == i_SuggestedBlock.hash)
            {
                if(i_SuggestedBlock.hash < this->m_DifficultyLimit)
                {
                    verified = true;
                }
                else
                {
                    std::string meassage = ", not eneogh leading 0's";
                    throw Exception("difficulty", meassage, i_SuggestedBlock);      
                }
            }
            else
            {
                std::string meassage = ", received 0x" + Exception::toHexString(i_SuggestedBlock.hash) + 
                                       " but calculated 0x" + Exception::toHexString(i_CheckSum);
                throw Exception("hash", meassage, i_SuggestedBlock);
            }
        }
        else
        {
            std::string meassage = ", received " + std::to_string(i_SuggestedBlock.height) + 
                                       " but expected " + std::to_string(g_BlockChainHead.height);
                throw Exception("height", meassage, i_SuggestedBlock);
        }     
    }
    else
    {
        std::string meassage = ", received 0x" + Exception::toHexString(i_SuggestedBlock.prev_hash) + 
                               " but expected 0x" + Exception::toHexString(g_BlockChainHead.hash) + "\n";
        throw Exception("prev_hash", meassage, i_SuggestedBlock);
    }    

    return verified;
}


void Server:: appendNewBlock(BLOCK_T& i_NewBlockChinHead) const
{
    pthread_mutex_lock(&g_HeadBlockLock);
    Server::s_ReadyToAppendBlock = true; 
    g_BlockChainHead = i_NewBlockChinHead;
    printBlockChainHead(i_NewBlockChinHead);
    Server::s_ReadyToAppendBlock = false;
    pthread_mutex_unlock(&g_HeadBlockLock);
    pthread_cond_broadcast(&g_ServerWriting); 
}

void Server:: printBlockChainHead(const BLOCK_T& i_BlockChainHead) const
{
    std::cout << "server: New block added by " << g_BlockChainHead.relayed_by << ", attributes: ";
    std::cout << "height(" << g_BlockChainHead.height << ") ";
    std::cout << "timestamp(" << g_BlockChainHead.timestamp << ") ";
    std::cout << "hash(" << std::hex << std::showbase << g_BlockChainHead.hash << ") ";
    std::cout << "prev_hash(" << g_BlockChainHead.prev_hash << std::dec <<") ";
    std::cout << "difficulty(" << g_BlockChainHead.difficulty << ") ";
    std::cout << "nonce(" << g_BlockChainHead.nonce << ") \n";
}