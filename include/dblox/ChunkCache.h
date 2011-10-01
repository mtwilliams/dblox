#ifndef DBLOX_CHUNK_CACHE_H
#define DBLOX_CHUNK_CACHE_H

#include "dblox/Chunk.h"

#include <string>
#include <map>

class World;
class ChunkCache
{
    public:
        ChunkCache( World* World );
        ~ChunkCache();

        Chunk* Load( int X, int Z );
        void Unload( int X, int Z );

    protected:
        World* m_pWorld;
        std::map<unsigned int, Chunk*> m_Chunks;
};

#endif
