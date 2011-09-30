#ifndef DBLOX_BLOCKS_EVENT_H
#define DBLOX_BLOCKS_EVENT_H

#define BLOCK_DIRTY 1

typedef enum
{
    BLOCK_EVENT_NULL = 0,
    BLOCK_EVENT_CREATED,
    BLOCK_EVENT_DESTROYED,
    BLOCK_EVENT_USED,
} BlockEventType;

typedef struct
{
    unsigned int Chunk[2];
    unsigned int Block[3];
} BlockPosition_t;

typedef struct
{
    BlockPosition_t Position;
} OnBlockCreated_t;

typedef struct
{
    BlockPosition_t Position;
} OnBlockDestroyed_t;

typedef struct
{
    // Entity* User;
} OnBlockUsed_t;

typedef struct
{
    BlockEventType Type;

    union
    {
        OnBlockCreated_t Created;
        OnBlockDestroyed_t Destroyed;
        OnBlockUsed_t Used;
    } Data;
} BlockEvent_t;

#endif