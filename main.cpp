#include "Server.hpp"
#include "Miner.hpp"
#include "Utils.hpp"

BLOCK_T g_SuggestedBlock;
BLOCK_T g_BlockChainHead;
pthread_mutex_t g_SuggestedBlockLock;
pthread_mutex_t g_HeadBlockLock;
pthread_cond_t g_newSuggestedBlock;
pthread_cond_t g_ServerWriting;
bool g_BlockNeedToBeChecked = false;

int main(int argc, char* argv[])
{
    int difficulty = 6;
    try
    {
        Exception::ValidateDifficulty(difficulty);
    }
    catch(const Exception& error)
    {
        error.Print();
        exit(1);
    }
    
    ulong difficultyLimit = pow((float)2,(float)(SIZE_OF_CRC_RESULT-difficulty));
    Server server;
    Miner miners[NUMBER_OF_MINERS];
    Miner dummyMiner;
    pthread_t serverThreadID;
    pthread_t minerThreadsIDs[NUMBER_OF_MINERS];
    pthread_t dummyMinerID;
    void* res;
    server.CreateGenesisBlock(g_BlockChainHead,difficulty);
    server.CreateGenesisBlock(g_SuggestedBlock,difficulty);
    pthread_mutex_init(&g_SuggestedBlockLock,NULL);
    pthread_mutex_init(&g_HeadBlockLock,NULL);
    pthread_cond_init(&g_newSuggestedBlock,NULL);
    pthread_cond_init(&g_ServerWriting,NULL);
    pthread_create(&serverThreadID,NULL,Server::ServerFlow,&server);
    server.SetDifficultyLimit(difficultyLimit);
    for (size_t i = 0; i < NUMBER_OF_MINERS; i++)
    {
        miners[i].SetMinerID(i + 1);
        miners[i].SetfficultyLimit(difficultyLimit);
        pthread_create(&minerThreadsIDs[i], NULL, Miner::MinerFlow, &miners[i]);
    }

    dummyMiner.SetMinerID(DUMMY_MINER_ID);
    dummyMiner.SetfficultyLimit(difficultyLimit);
    pthread_create(&dummyMinerID, NULL, Miner::MinerFlow, &dummyMiner);
    for (size_t i = 0; i < NUMBER_OF_MINERS; i++)
    {
       pthread_join(minerThreadsIDs[i],&res);
    }

    pthread_join(dummyMinerID,&res);
    pthread_join(serverThreadID,&res);
}