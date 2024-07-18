/* Basic arena.

Features:
- fixed capacity
- no support for allignment
- panic when run out of memory
- malloc upstream
*/
// #include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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
    void *result = (char *)a->data + a->used;

    if(a->used + size > a->capacity) {
        PANIC();
    }

    a->used += size;

    return result;
}

void arena_clear(Arena *a)
{
    a->used = 0;
}

size_t arena_pos(Arena *a)
{
    return a->used;
}

void arena_pop_to(Arena *a, size_t pos)
{
    assert(pos <= a->capacity);
    a->used = pos;
}

void arena_pop_size(Arena *a, size_t size)
{
    assert(size <= a->used);
    a->used -= size;
}

void arena_set_allign(Arena *a, int allign)
{
    (void)a;
    (void)allign;
}

void arena_print_report(Arena *a)
{
    printf("capacity: %ld\n", a->capacity);
    printf("used: %ld\n", a->used);
}