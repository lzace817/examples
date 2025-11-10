/* Allign arena

Features:
- dinamic capacity
- malloc upstream
- no support to stack like allocations
- support allignment
*/
// #include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h> //memset
#include "arena.h"

void * arena_push_size_no_zero(Arena *a, size_t size);


#define MAX(v1, v2) ((v1) > (v2))? (v1) : (v2)
#define IS_POW2(num) ((num) > 0 )?(((num)-1) & (num)) == 0:0

#define DEFAULT_CHUNK_CAPACITY 4*1024

typedef struct Chunk Chunk;
struct Chunk {
    Chunk *next;
    size_t capacity;
    void *data;
};

struct Arena {
    Chunk *first;
    Chunk *curent;
    size_t pos;
    int allign;
};

Arena *arena_create(void)
{
    Arena *result = malloc(DEFAULT_CHUNK_CAPACITY + sizeof(Chunk) + sizeof(Arena));
    Chunk *chunk = (void *)(&result[1]);
    *result = (Arena){
        .first = chunk,
        .curent = chunk,
    };
    *chunk = (Chunk){
        .data = (void *)(&chunk[1]),
        .capacity = DEFAULT_CHUNK_CAPACITY,
    };
    return result;
}

void arena_release(Arena *a)
{
    Chunk *c = a->first->next;
    while(c) {
        Chunk *n = c->next;
        free(c);
        c = n;
    }
    free(a); // arena + fist chunk
}

static int padding_foward(uintptr_t addr, int allign)
{
    if (!allign) return 0;
    assert(IS_POW2(allign));
    int result = ((addr + allign -1) & (~(allign-1))) - addr;
    return result;
}

void * arena_push_size_no_zero(Arena *a, size_t size)
{
    size_t pos = a->pos;
    Chunk *chunk = a->curent;
    void *result;
    int padding = 0;

    while(chunk){
        padding = padding_foward((uintptr_t)chunk->data + pos, a->allign);
        if(pos + size + padding <= chunk->capacity){
            break;
        }
        chunk = chunk->next;
        pos = 0;
    }
    if(!chunk) {
        size_t newcap = MAX(DEFAULT_CHUNK_CAPACITY, size);
        chunk = malloc(newcap + sizeof(Chunk) + a->allign);
        *chunk = (Chunk){
            .capacity = newcap,
            .data = &chunk[1],
        };
        assert(a->curent->next == NULL);
        a->curent->next = chunk;
        a->curent = chunk;
        pos = 0;
        padding = padding_foward((uintptr_t)chunk->data, a->allign);
    }
    result = (char *)chunk->data + pos + padding;
    a->pos = pos + size + padding;
    return result;
}

void * arena_push_size(Arena *a, size_t size)
{
    void * result = arena_push_size_no_zero(a, size);
    memset(result, 0, size);
    return result;
}


void arena_clear(Arena *a)
{
    a->curent = a->first;
    a->pos = 0;
}

size_t arena_pos(Arena *a)
{
    assert(0 && "not implemented");
}

void arena_pop_to(Arena *a, size_t pos)
{
    assert(0 && "not implemented");
}

void arena_pop_size(Arena *a, size_t size)
{
    assert(0 && "not implemented");
}

void arena_set_allign(Arena *a, int allign)
{
    a->allign = allign;
}

static size_t get_num_chunks(Arena *a)
{
    size_t n = 0;
    for(Chunk *c = a->first; c ; c = c->next) {
        n++;
    }
    return n;
}

void arena_print_report(Arena *a)
{
    size_t num_chunks = get_num_chunks(a);
    printf("# Chunks: %ld\n", num_chunks);
    int i = 0;
    size_t total_cap = 0;
    for(Chunk *c = a->first; c ; c = c->next) {
        printf("    chunk #%d have cap %ld\n", i++, c->capacity);
        total_cap += c->capacity;
    }
    printf("total capacity: %ld\n", total_cap);
}