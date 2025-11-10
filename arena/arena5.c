/* Temp arena

Features:
- fixed capacity
- no support for allignment
- panic when run out of memory
- static storage
*/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "arena.h"

#define PANIC() do {                                           \
        fprintf(stderr, "%s:%d: PANIC\n", __FILE__,__LINE__);  \
        exit(1);                                               \
    } while(0)

struct Arena {
    void *data;
    size_t size;
    size_t capacity;
};

#define DEFAULT_ARENA_CAPACITY 8*1024
static char temp_data[DEFAULT_ARENA_CAPACITY];
static Arena temp = {.data = temp_data, .capacity = DEFAULT_ARENA_CAPACITY};

Arena *arena_create(void)
{
    temp.size = 0;
    return &temp;
}

void arena_release(Arena *a)
{
}

void * arena_push_size(Arena *a, size_t size)
{
    void *result = (char *)a->data + a->size;

    if(a->size + size > a->capacity) {
        PANIC();
        return NULL;
    }

    a->size += size;

    return result;
}

void arena_clear(Arena *a)
{
    a->size = 0;
}

size_t arena_pos(Arena *a)
{
    return a->size;
}

void arena_pop_to(Arena *a, size_t pos)
{
    assert(pos <= a->capacity);
    a->size = pos;
}

void arena_pop_size(Arena *a, size_t size)
{
    assert(size <= a->size);
    a->size -= size;
}

void arena_set_allign(Arena *a, int allign)
{
    (void)a;
    (void)allign;
}

void arena_print_report(Arena *a)
{
    printf("capacity: %ld\n", a->capacity);
    printf("used: %ld\n", a->size);
}
