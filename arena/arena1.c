// #include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "arena.h"

#define PANIC() do {                                           \
        fprintf(stderr, "%s:%d: PANIC\n", __FILE__,__LINE__);  \
        exit(1);                                               \
    } while(0)

#define DEFAULT_ARENA_CAPACITY 1024

struct Arena {
    void *data;
    size_t capacity;
    size_t used;
};

Arena *arena_create(void)
{
    Arena *result = malloc(DEFAULT_ARENA_CAPACITY);

    result->data = &result[1];
    result->used = 0;
    result->capacity = DEFAULT_ARENA_CAPACITY - sizeof(Arena);

    return result;
}

void arena_release(Arena *a)
{
    free(a);
}

void * arena_push_size(Arena *a, size_t size)
{
    void *result = a->data + a->used;

    if(a->used + size > a->capacity) {
        PANIC();
    }

    a->used += size;

    return result;
}

void arena_free_all(Arena *a)
{
    a->used = 0;
}

size_t arena_used(Arena *a)
{
    return a->used;
}

void arena_set_allign(Arena *a, int allign)
{
    (void)a;
    (void)allign;
}