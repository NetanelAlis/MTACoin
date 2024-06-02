#include "Server.hpp"
#include "Miner.hpp"
#include "Utils.hpp"
#define NUMBER_OF_MINERS 4

BLOCK_T g_SuggestedBlock;
BLOCK_T g_BlockChainHead;
pthread_mutex_t g_SuggestedBlockLock;
pthread_cond_t g_newSuggestedBlock;

int main()
{
    Server server;
    Miner miners[NUMBER_OF_MINERS];
    pthread_t serverThreadID;
    pthread_t minerThreadsIDs[NUMBER_OF_MINERS];
    void* res;

    server.CreateGenesisBlock(g_BlockChainHead);
    server.CreateGenesisBlock(g_SuggestedBlock);
    pthread_mutex_init(&g_SuggestedBlockLock,NULL);
    pthread_cond_init(&g_newSuggestedBlock,NULL);

    pthread_create(&serverThreadID,NULL,Server::ServerFlow,&server);
    for (size_t i = 0; i < NUMBER_OF_MINERS; i++)
    {
        miners[i].SetMinerID(i + 1);
        pthread_create(&minerThreadsIDs[i], NULL, Miner::MinerFlow, &miners[i]);
    }
    

    
    for (size_t i = 0; i < NUMBER_OF_MINERS; i++)
    {
        pthread_join(minerThreadsIDs[i], &res);  
    }

    return 0;
}