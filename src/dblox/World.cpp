#include "Utils/Coordinates.h"
#include "dblox/World.h"
#include "dblox/Chunk.h"
#include "dblox/ChunkCache.h"
#include "dblox/PerlinNoise.h"
#include "dblox/WorldRenderer.h"

World::World( unsigned int Seed )
    : m_Seed(Seed)
    , m_pRenderer(0)
{
    m_pRenderer = new WorldRenderer();

    static const unsigned int Chunks = 7;
    static const unsigned int Center = 0;//Chunks / 2;

    for( int x = 0; x < Chunks; ++x ) {
    for( int z = 0; z < Chunks; ++z ) {
        m_Chunks.insert(std::make_pair(Utils::GetCantorsPair(x - Center, z - Center), new Chunk(x - Center, z - Center, this)));
        Generate(x - Center, z - Center);
    }
    }
}
       
World::~World()
{
}

bool World::Initialize( void )
{
    if( !m_pRenderer->Initialize() )
        return false;

    return true;
}

void World::Update( float DeltaTime )
{
}

void World::Render( void )
{
    for( std::map<unsigned int, Chunk*>::iterator Iter = m_Chunks.begin(); Iter != m_Chunks.end(); ++Iter )
    {
        m_pRenderer->AddChunkToRenderQueue(Iter->second);
    }
    m_pRenderer->Render();
}

void World::Generate( unsigned int X, unsigned int Z )
{
    PerlinNoise Noise = PerlinNoise(m_Seed);

    std::map<unsigned int, Chunk*>::const_iterator Iter = m_Chunks.find(Utils::GetCantorsPair(X, Z));

    if( Iter == m_Chunks.end() )
        return;

    for( unsigned int x = 0; x < Chunk::Width; ++x ) {
    for( unsigned int z = 0; z < Chunk::Width; ++z ) {

        double N = Noise.SmoothNoise2D((x + X * Chunk::Width) / 16.0, (z + Z * Chunk::Width) / 16.0, 1.0 / 2.0, 2);
        unsigned int Ground = (Chunk::Height / 2 - 1) + ((Chunk::Height / 8) * N);
        unsigned int Stone = (Chunk::Height / 2 - 1) - (6 - (N * 2 + 2));
        Stone = (Stone + Ground) / 2;

    for( unsigned int y = 0; y < Chunk::Height; ++y ) {
        if( y < Ground )
        {
            if( y < Stone )
                Iter->second->Blocks[x][z][y] = Blocks::Stone;
            else
                Iter->second->Blocks[x][z][y] = Blocks::Dirt;
        }
        else if( y == Ground )
        {
            Iter->second->Blocks[x][z][y] = Blocks::Grass;
        }
        else
        {
            Iter->second->Blocks[x][z][y] = Blocks::Air;
        }
    }}}

    Iter->second->Rebuild();
}
