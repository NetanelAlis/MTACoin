#include "Server.hpp"
#include "Miner.hpp"

void* Miner::MinerFlow(void* i_Miner)
{
    Miner* miner = static_cast<Miner*>(i_Miner);

    miner->Mine();

    return i_Miner;
}

void Miner::Mine()
{
    while (true)
    {
        pthread_mutex_lock(&g_HeadBlockLock);
        if(Server::s_ReadyToAppendBlock)
        {
            pthread_cond_wait(&g_ServerWriting,&g_HeadBlockLock); 
        }
        readHeadBlock();
        pthread_mutex_unlock(&g_HeadBlockLock);
        getHash();
        if(isValidHash())
        {
            m_BlockWasCheckedByTheServer = false;
            while(m_BlockWasCheckedByTheServer == false)
            {
                pthread_mutex_lock(&g_SuggestedBlockLock);
                if(g_BlockNeedToBeChecked == true)
                {
                    pthread_cond_wait(&g_newSuggestedBlock,&g_SuggestedBlockLock);
                }
                if(g_BlockNeedToBeChecked == false)
                {
                    suggestBlock();
                    printSuggestion();
                }
                pthread_mutex_unlock(&g_SuggestedBlockLock);
                pthread_cond_broadcast(&g_newSuggestedBlock);
            }

        }

    }

}

void Miner:: readHeadBlock()
{
    if(m_PrevHash != g_BlockChainHead.hash)
    {
        m_BlockHeight = g_BlockChainHead.height+1; 
        m_Difficulty = g_BlockChainHead.difficulty;   
        m_PrevHash = g_BlockChainHead.hash; 
        m_Nonce = 0;//check if we want to init it each time 
    }
   
}

    void Miner::getHash()
{
    m_Nonce++;
    m_Timestamp = std::time(nullptr);
    unsigned int crcParams[] = {m_BlockHeight,
                                m_Timestamp,
                                m_PrevHash,
                                m_Nonce,
                                m_MinerID};

    m_Hash = crc32(m_PrevHash, (const Bytef*)crcParams, sizeof(crcParams));
}

bool Miner::isValidHash()
{
    bool verified = false;

    if(m_Hash < m_DifficultyLimit)
    {
        verified = true;
    }

    return verified;
}

void Miner::suggestBlock()
{
   g_SuggestedBlock.height = m_BlockHeight;
   g_SuggestedBlock.timestamp = m_Timestamp;
   g_SuggestedBlock.hash = m_Hash;
   g_SuggestedBlock.prev_hash = m_PrevHash;
   g_SuggestedBlock.difficulty = m_Difficulty;
   g_SuggestedBlock.nonce = m_Nonce;
   g_SuggestedBlock.relayed_by = m_MinerID;
   SetBlockWasCheckedByTheServer(true);
   g_BlockNeedToBeChecked = true; 
}

void Miner::printSuggestion() const
{
    std::cout << "Miner #" << m_MinerID;
    std::cout << ": mined a new block #" << m_BlockHeight;
    std::cout << " with the hash " <<  std::hex << std::showbase << m_Hash << std::dec << std::endl;
}