#ifndef ARENA_H
#define ARENA_H

typedef struct Arena Arena;

Arena* arena_create(void);
void   arena_release(Arena *a);
void * arena_push_size(Arena *a, size_t size);
void   arena_clear(Arena *a);
size_t arena_pos(Arena *a);
void   arena_pop_to(Arena *a, size_t pos);
void   arena_pop_size(Arena *a, size_t size);
void   arena_set_allign(Arena *a, int allign);

#endif // ARENA_H