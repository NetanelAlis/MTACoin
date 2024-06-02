#ifndef UTILS_HPP
#define UTILS_HPP

#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <zlib.h>
#include <math.h>

struct BLOCK_T {
    int height;        // Incremental ID of the block in the chain
    int timestamp;     // Time of the mine in seconds since epoch
    unsigned int hash; // Current block hash value
    unsigned int prev_hash; // Hash value of the previous block
    int difficulty;    // Amount of preceding zeros in the hash
    int nonce;         // Incremental integer to change the hash value
    int relayed_by;    // Miner ID

    bool operator==(const BLOCK_T& other) const {
        return (height == other.height) &&
               (timestamp == other.timestamp) &&
               (hash == other.hash) &&
               (prev_hash == other.prev_hash) &&
               (difficulty == other.difficulty) &&
               (nonce == other.nonce) &&
               (relayed_by == other.relayed_by);
    }

    bool operator!=(const BLOCK_T& other) const {
        return !(*this == other);
    }
};

extern BLOCK_T g_SuggestedBlock;
extern BLOCK_T g_BlockChainHead; 
extern pthread_mutex_t g_SuggestedBlockLock;
extern pthread_cond_t g_newSuggestedBlock;

#endif // UTILS_HPP
