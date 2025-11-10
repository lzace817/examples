/* Allign arena

Features:
- 1G max capacity
- panic when run out of memory
- mmap upstream
- alloc new contiguous blocks as neded but never moves
*/
// #include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h> //memset
#include <sys/mman.h>
#include "arena.h"

void * arena_push_size_no_zero(Arena *a, size_t size);

#define ALIGN_DOWN(n, a) ((n) & ~((a) - 1))
#define ALIGN_UP(n, a) ALIGN_DOWN((n) + (a) - 1, (a))
#define ALIGN_DOWN_PTR(p, a) ((void *)ALIGN_DOWN((uintptr_t)(p), (a)))
#define ALIGN_UP_PTR(p, a) ((void *)ALIGN_UP((uintptr_t)(p), (a)))

#define RESERVE_SIZE (1L*1024*1024*1024)
#define DEFAULT_ARENA_CAPACITY 4*1024
struct Arena {
    void *data;
    size_t capacity;
    size_t mapped_cap;
    size_t used;
    int allign;
    int shrink;
};

Arena *arena_create(void)
{
    Arena *result = mmap(0, RESERVE_SIZE,
                    PROT_NONE,
                    MAP_ANONYMOUS | MAP_PRIVATE,
                    -1, 0);
    if (result == MAP_FAILED) {
        perror("ncmem");
        exit(1);
    }
    int status = mprotect(result, DEFAULT_ARENA_CAPACITY, PROT_WRITE | PROT_READ);
    if(status == -1) {
        perror("ncmem");
        exit(1);
    }

    result->data = &result[1];
    result->used = 0;
    result->capacity = DEFAULT_ARENA_CAPACITY - sizeof(Arena);
    result->allign = 0;
    result->mapped_cap = DEFAULT_ARENA_CAPACITY;

    return result;
}

void arena_release(Arena *a)
{
    // arena_print_report(a);
    int status = munmap(a, RESERVE_SIZE);
    if(status == -1) {
        perror("ncmem");
        exit(1);
    }
}

void * arena_push_size_no_zero(Arena *a, size_t size)
{
    int start = a->used;
    void *result = (char *)a->data + start;
    int padding = 0;
    if(a->allign) {
        padding = (char*)ALIGN_UP_PTR(result, a->allign) - (char*)result;
        result = (char*)result + padding;
    }
    if(start + size + padding > a->capacity){
        // grow
        size_t grow_ammount = start + size + padding - a->capacity;
        grow_ammount = ALIGN_UP(grow_ammount, DEFAULT_ARENA_CAPACITY);
        assert(a->mapped_cap + grow_ammount <= RESERVE_SIZE);
        a->mapped_cap += grow_ammount;
        a->capacity += grow_ammount;
        int status = mprotect(a, a->mapped_cap,
                        PROT_WRITE | PROT_READ);
        if(status == -1){
            perror("ncmem");
            exit(1);
        }
    }
    a->used = start + size + padding;

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
#if 1
    if(a->shrink && (a->mapped_cap - DEFAULT_ARENA_CAPACITY) > 0) {
        // this even works?
        // int status = mprotect(a + DEFAULT_ARENA_CAPACITY,
        //         a->mapped_cap - DEFAULT_ARENA_CAPACITY,
        //         PROT_WRITE | PROT_READ);
        int status = madvise(a + DEFAULT_ARENA_CAPACITY,
                a->mapped_cap - DEFAULT_ARENA_CAPACITY,
                MADV_DONTNEED);
        if(status == -1){
            perror("ncmem");
            exit(1);
        }
        // a->mapped_cap = DEFAULT_ARENA_CAPACITY;
        // a->capacity = DEFAULT_ARENA_CAPACITY - sizeof(Arena);
    }
#endif
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
    printf("mapped_cap: %ld\n", a->mapped_cap);
}