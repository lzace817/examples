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

/* Set the allignment for all subsequent allocations.
if allign is zero mean no allignment.
*/
void   arena_set_allign(Arena *a, int allign);

/* print information about the curent internal state of allocator.
*/
void   arena_print_report(Arena *a);

#endif // ARENA_H