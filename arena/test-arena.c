#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "arena.h"

void test_memory(void *data, size_t size)
{
    uint8_t *mem = (uint8_t *)data;

    for(size_t i = 0; i < size; i++) {
        mem[i] = i;
    }

    for(size_t i = 0; i < size; i++) {
        assert(mem[i] == i);
    }
}

int test1(void)
{
    Arena *arena = arena_create();

    for(int i = 0; i < 10; i++) {
        test_memory(arena_push_size(arena, 100), 100);
    }

    arena_clear(arena);

    for(int i = 0; i < 12; i++) {
        test_memory(arena_push_size(arena, 64), 64);
    }

    arena_release(arena);

    printf("test 1 PASSED\n");
    return 0;
}

int test2(void)
{
    Arena *arena = arena_create();
    arena_set_allign(arena, 8);

    size_t expected_use = 3;
    for (int i = 0; i < 10; i++) {
        arena_push_size(arena, 3);
        size_t used = arena_pos(arena);
        // printf("allocated +3. used = %d\n", used);
        assert(used == expected_use);
        expected_use += 8;
    }

    expected_use = arena_pos(arena) + 9;

    arena_set_allign(arena, 0);
    arena_push_size(arena, 7);
    arena_push_size(arena, 2);
    assert(arena_pos(arena) == expected_use);

    arena_release(arena);
    printf("test 2 PASSED\n");

    return 0;
}

int test3(void)
{
    // NOTE(proto): test pop procs

    Arena *a = arena_create();

    arena_push_size(a, 100);
    size_t pos = arena_pos(a);
    arena_push_size(a, 100);
    assert(arena_pos(a) == 200);
    arena_pop_to(a, pos);
    assert(arena_pos(a) == 100);
    arena_release(a);

    printf("test 3 PASSED\n");

    return 0;
}

int test4(void)
{
    Arena *a = arena_create();
    char *b = arena_push_size(a, 1);
    (void)b;

    // char *c = arena_push_size(a, 1);
    // (void)c;

    arena_set_allign(a, sizeof(int));
    int *i32 = arena_push_size(a, sizeof(int));
    assert((uintptr_t)i32 % sizeof(int) == 0);

    arena_set_allign(a, sizeof(int64_t));
    int64_t *i64 = arena_push_size(a, sizeof(int64_t));
    assert((uintptr_t)i64 % sizeof(int64_t) == 0);

    arena_release(a);

    printf("test 4 PASSED\n");
    return 0;
}

int test5(void)
{
    Arena *a = arena_create();
    uint64_t n = 10000;

    arena_set_allign(a, 8);
    uint64_t *p = arena_push_size(a, sizeof(p)*n);

    for(uint64_t i = 0; i < n; i++) {
        p[i] = i*i + 123;
    }

    for(uint64_t i = 0; i < n; i++) {
        assert(p[i] == i*i + 123);
    }

    arena_release(a);

    printf("test 5 PASSED\n");
    return 0;
}

int main(void)
{
    // test1();
    // test2();
    // test3();
    // test4();
    test5();

    return 0;
}