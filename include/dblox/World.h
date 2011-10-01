#ifndef DBLOX_WORLD_H
#define DBLOX_WORLD_H

#include <map>

class Chunk;
class ChunkCache;
class WorldRenderer;

class World
{
    public:
        World( unsigned int Seed );
        ~World();

        bool Initialize( void );

        void Update( float DeltaTime );
        void Render( void );

        void Generate( unsigned int X, unsigned int Z );

    protected:
        unsigned int m_Seed;
        WorldRenderer* m_pRenderer;

        std::map<unsigned int, Chunk*> m_Chunks;
};

#endif
