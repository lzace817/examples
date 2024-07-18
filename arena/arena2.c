/* Allign arena

Features:
- fixed capacity
- panic when run out of memory
- malloc upstream
*/
// #include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h> //memset
#include "arena.h"

#define DEFAULT_ARENA_CAPACITY (1024+sizeof(Arena))

struct Arena {
    void *data;
    size_t capacity;
    size_t used;
    int allign;
};

Arena *arena_create(void)
{
    Arena *result = malloc(DEFAULT_ARENA_CAPACITY);

    result->data = &result[1];
    result->used = 0;
    result->capacity = DEFAULT_ARENA_CAPACITY - sizeof(Arena);
    result->allign = 0;

    return result;
}

void arena_release(Arena *a)
{
    free(a);
}

void * arena_push_size_no_zero(Arena *a, size_t size)
{
    int start = a->used;
    if (a->allign > 0) {
        assert(((a->allign-1) & a->allign) == 0);
        start += (a->allign - (a->used % a->allign)) % a->allign;
    }
    void *result = (char *)a->data + start;
    assert(start + size <= a->capacity);
    a->used = start + size;

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
    a->allign = allign;
}

void arena_print_report(Arena *a)
{
    printf("capacity: %ld\n", a->capacity);
    printf("used: %ld\n", a->used);
}