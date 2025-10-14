/** # common stuff for C

    - dynamic arrays
    - arena allocator
    - and other basics

    ## dynamic arrays

    using libc:
    - da_append(da, x)
    - da_append_many(da, items, num_items)
    - da_pop(da)
    - da_free(da)
    - sb_append_cstr(sb, str)

    and the version using arena as allocator
    - arena_da_append(arena, da, x)
    - arena_da_append_many(arena, da, items, num_items)
    - arena_sb_append_cstr(arena, sb, str)

    ## arena allocator

    void * arena_push_size(Arena *a, size_t size);
    void   arena_reset(Arena *a);
    void   arena_free(Arena *a);
    Arena_Mark arena_snapshot(Arena *a);
    void   arena_rewind(Arena *a, Arena_Mark m);
    void   arena_set_allign(Arena *a, int allign);
    void   arena_status(Arena *a);

    ## miscelaneous

    - ARRAYSIZE(array)
    - internal
    - global_variable
    - TODO(msg)
    - UNREACHEABLE(fmt)


    ## TODO
    -[ ] hex_string <-> byte array and xxd style hexdump
    -[ ] base64     <-> byte array
    -[ ] sb_appendf(sb, fmt, ...)
 */
#ifndef COMMONS_H
#define COMMONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define ARRAYSIZE(array) sizeof(array) / sizeof(*(array))
#define internal static
#define global_variable static

#define TODO(msg) do{fprintf(stderr, "%s:%i: TODO: " msg "\n", __FILE__,      \
        __LINE__ ); abort();}while(0)

#define UNREACHEABLE(fmt) do{ fprintf(stderr, "%s:%i: UNREACHEABLE: "         \
        fmt "\n", __FILE__, __LINE__); abort();} while(0)

/* Dynamic array:
 *
 *     da_append(da, item)      | adds item to da
 *
 */

 typedef struct {
    char* items;
    size_t size;
    size_t capacity;
} String_Builder;

#define da_append(da, item)                                \
    do {                                                   \
        da_reserve((void*)&(da)->items, &(da)->capacity,   \
                (da)->size+1, sizeof(*(da)->items));       \
        (da)->items[(da)->size++] = (item);                \
    } while(0)

#define da_append_many(da, _items, num_items)                                           \
    do {                                                                                \
        da_reserve((void*)&(da)->items, &(da)->capacity,                                \
                (da)->size + (num_items), sizeof(*(da)->items));                        \
        memcpy((da)->items + (da)->size, (_items), sizeof(*(da)->items) * num_items);   \
        (da)->size += (num_items);                                                      \
    } while(0)


#define sb_append_cstr(sb, cstr)                    \
    do {                                            \
        size_t cstr_size = strlen((cstr));          \
        da_append_many((sb), (cstr), cstr_size);    \
    } while(0)


//
// Arena
//

#define BLOCK_SIZE (4*1024)

typedef struct Block Block;
struct Block{
    Block *next;
    size_t capacity;
    uint8_t data[];
};

typedef struct Arena Arena;
struct Arena {
    Block *begin;
    Block *end;
    size_t size;
    int allign;
};

typedef struct Arena_Mark Arena_Mark;
struct Arena_Mark {
    Block *block;
    size_t size;
};

void * arena_push_size(Arena *a, size_t size);
void   arena_reset(Arena *a);
void   arena_free(Arena *a);
Arena_Mark arena_snapshot(Arena *a);
void   arena_rewind(Arena *a, Arena_Mark m);

/* Set the allignment for all subsequent allocations.
if allign is zero mean no allignment.
*/
void   arena_set_allign(Arena *a, int allign);

/* print information about the curent internal state of allocator.
*/
void   arena_status(Arena *a);

/*   # arena da
*/
#define arena_da_append(arena, da, item)                                   \
    do {                                                                   \
        arena_da_reserve((arena), (void*)&(da)->items, &(da)->capacity,    \
                (da)->size+1, sizeof(*(da)->items));                       \
        (da)->items[(da)->size++] = (item);                                \
    } while(0)

void da_reserve(void** items, size_t *capacity, size_t desired, size_t item_size);
// string view
typedef struct {
    const char* items;
    size_t size;
} String_View;

#define SV_fmt "%.*s"
#define SV_arg(sv) (int)(sv)->size, (sv)->items
#define SV(strlit) ((String_View){.size=sizeof(strlit)-1, .items=strlit})

bool sv_starts_with(String_View sv, String_View prefix);
bool sv_try_chop_by_delim(String_View* sb, const char delim, String_View* chunk);
String_View sb_to_sv(String_Builder sb);
String_View sv_trim_right(String_View sv);
String_View sv_trim_left(String_View sv);
String_View sv_trim_both(String_View sv);
String_View sv_from_parts(const char* items, size_t size);

void sb_append_uint(String_Builder* sb, size_t val, int base, int min_digits);
void sb_append_as_hex(String_Builder *dest, const String_View src);
void sb_append_ascii(String_Builder* dst, String_View src);
void sb_append_hex_dump(String_Builder* dst, String_View src);

bool read_entire_file(const char* path, String_Builder *sb);

////    end header file    ////////////////////////////////////////////////////
#endif // COMMONS_H

#ifdef COMMONS_IMPLEMENTATION
#undef COMMONS_IMPLEMENTATION

void da_reserve(void** items, size_t *capacity, size_t desired, size_t item_size)
{
    if (desired <= *capacity) return;
#define DA_BASE_CAP 8
    if (*capacity == 0) *capacity = DA_BASE_CAP;
    while (*capacity < desired) (*capacity) *= 2;
    *items = realloc(*items, *capacity * item_size);
    assert(*items);
}

bool read_entire_file(const char* path, String_Builder *sb)
{
    bool result = true;
    FILE* f = fopen(path, "rb");
    if(!f) {
        result = false;
        goto cleanup;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        result = false;
        goto cleanup;
    }

    long file_size = ftell(f);
    if(file_size < 0) {
        result = false;
        goto cleanup;
    }

    if (fseek(f, 0, SEEK_SET) != 0) {
        result = false;
        goto cleanup;
    }

    da_reserve((void*)&sb->items, &sb->capacity, sb->size + file_size, sizeof(*sb->items));
    if (fread(sb->items + sb->size, file_size, 1, f) == 0) {
        if(ferror(f)) {
            return false;
            goto cleanup;
        }
    }
    sb->size += file_size;

cleanup:
    if (!result) perror("read_entire_file");
    if (f) fclose(f);
    return result;
}


internal Block *new_block(size_t desired)
{
    size_t to_allocate = desired + sizeof(Block);
    size_t num_blocks = (to_allocate + BLOCK_SIZE - 1) / BLOCK_SIZE;
    to_allocate = num_blocks*BLOCK_SIZE;

    Block *result = (Block *)malloc(to_allocate);

    result->capacity = to_allocate - sizeof(Block);
    result->next = NULL;

    return result;
}

void * arena_push_size(Arena *a, size_t size)
{
    static_assert(sizeof(Block) % 8 == 0);
    int allign = (a->allign)? a->allign : 1;

    if(a->end == NULL) {
        assert(a->begin == NULL);
        Block *b = new_block(size);
        a->begin = b;
        a->end = b;
        a->size = 0;
    }

    size_t offset = ((a->size + allign -1) / allign) * allign;

    while(offset + size > a->end->capacity && a->end->next != NULL) {
        a->end = a->end->next;
        offset = 0;
    }

    if(offset + size > a->end->capacity) {
        assert(a->end->next == NULL);
        Block *b = new_block(size);
        a->end->next = b;
        a->end = b;
        a->size = 0;
        offset = 0;
    }

    void *result = a->end->data + offset;
    a->size = offset + size;

    return result;
}

void   arena_reset(Arena *a)
{
    a->end = a->begin;
    a->size = 0;
}

void   arena_free(Arena *a)
{
    Block *p = a->begin;
    Block *temp;
    while(p != NULL) {
        temp = p;
        p = p->next;
        free(temp);
    }
    a->begin = NULL;
    a->end   = NULL;
}

Arena_Mark arena_snapshot(Arena *a)
{

    Arena_Mark result = {a->end, a->size};
    return result;
}

void   arena_rewind(Arena *a, Arena_Mark m)
{
    if (m.block == NULL)
        a->end = a->begin;
    else
        a->end = m.block;
    a->size = m.size;
}

void   arena_set_allign(Arena *a, int allign)
{
    assert( allign == 0 || allign == 1 || allign == 2 || allign == 4
            || allign == 8 );
    a->allign = allign;
}

void   arena_status(Arena *a)
{
    size_t num_blocks = 0;
    size_t total_capacity = 0;
    int cur_block = -1;
    for(Block *p = a->begin; p != NULL; p = p->next) {
        num_blocks += 1;
        total_capacity += p->capacity;
        if (p == a->end) cur_block = num_blocks;
    }
    printf("block header: %zu\n", sizeof(Block));
    printf("block size: %u\n", BLOCK_SIZE);
    printf("%zu blocks with capacity:\n", num_blocks);
    num_blocks = 0;
    for(Block *p = a->begin; p != NULL; p = p->next) {
        printf("    #%zu: %zu\n", ++num_blocks, p->capacity);
    }
    if (cur_block > 0) {
        printf("curent block (#%d):\n    used: %zu\n    available:%zu\n    "
                "capacity: %zu\n", cur_block, a->size, a->end->capacity -
                a->size, a->end->capacity);
    }
    printf("total capacity: %zu\n", total_capacity);
    printf("\n");

}

internal void arena_da_reserve(Arena *a, void** items, size_t *capacity, size_t desired, size_t item_size)
{
    if (desired <= *capacity) return;
    size_t old_cap = *capacity;
#define DA_BASE_CAP 8
    if (*capacity == 0) *capacity = DA_BASE_CAP;
    while (*capacity < desired) (*capacity) *= 2;
    void* old_items = *items;
    *items = arena_push_size(a, (*capacity) * item_size);
    assert(*items);
    memcpy(*items, old_items, old_cap);
}

//sv
bool sv_try_chop_by_delim(String_View* sb, const char delim, String_View* chunk)
{
    const char* cursor;
    const char* end = sb->items + sb->size;

    for(cursor = sb->items; cursor < end; cursor++) {
        if(*cursor == delim) break;
    }

    String_View result = sv_from_parts(sb->items, cursor - sb->items);
    if (cursor < end) {
        sb->items += result.size + 1;
        sb->size  -= result.size + 1;
        if (chunk) {
            *chunk = result;
        }
        return true;
    }

    return false;
}

String_View sb_to_sv(String_Builder sb)
{
    return (String_View){.items = sb.items, .size = sb.size};
}

bool sv_starts_with(String_View sv, String_View prefix)
{
    if(prefix.size > sv.size) return false;
    return strncmp(sv.items, prefix.items, prefix.size) == 0;
}

String_View sv_trim_right(String_View sv)
{
    while((sv.size) && isspace(sv.items[--sv.size])) ;
    if(!isspace(sv.items[sv.size])) sv.size++;
    return sv;
}

String_View sv_trim_left(String_View sv)
{
    while(sv.size && isspace(*sv.items)) {
        sv.size--;
        sv.items++;
    }
    return sv;
}

String_View sv_trim_both(String_View sv)
{
    return sv_trim_left(sv_trim_right(sv));
}

String_View sv_from_parts(const char* items, size_t size)
{
    return (String_View){.items = items, .size = size};
}

void sb_append_as_hex(String_Builder *dest, const String_View src)
{
    // const char hex_digits[] = "0123456789ABCDEF";
    const char hex_digits[] = "0123456789abcdef";
    for(size_t i = 0; i < src.size; i++) {
        unsigned char byte = src.items[i];
        da_append(dest, hex_digits[(byte >> 4) & 0xf]);
        da_append(dest, hex_digits[(byte >> 0) & 0xf]);
    }
}

void sb_append_uint(String_Builder* sb, size_t val, int base, int min_digits)
{
    const char hex_digits[] = "0123456789abcdef";

    unsigned char digits[64];
    assert(base >= 2 && base <= 16);
    assert(min_digits <= 64);

    memset(digits, '0', sizeof(digits));

    int dlen = 0;
    while(val) {
        digits[dlen++] = hex_digits[val % base];
        val = val / base;
    }

    if(min_digits > dlen) dlen = min_digits;

    da_reserve((void*)&sb->items, &sb->capacity, sb->size + dlen, sizeof(*sb->items));

    while(dlen) {
        da_append(sb, digits[--dlen]);
    }
}

void sb_append_ascii(String_Builder* dst, String_View src)
{
    for(size_t i = 0; i < src.size; i++){
        unsigned char c = src.items[i];
        da_append(dst, (c >= 32 && c < 128)? c : '.');
    }
}

#define BYTES_BY_LINE 16
#define BYTES_BY_COL  2
void sb_append_hex_dump(String_Builder* dst, String_View src)
{
    size_t line;
    for(line = 0; line + BYTES_BY_LINE <= src.size; line+=BYTES_BY_LINE) {
        sb_append_uint(dst, line, 16, 8);
        da_append_many(dst, ": ", 2);
        size_t col;
        for(col = 0; col + BYTES_BY_COL <= BYTES_BY_LINE;
            col+=BYTES_BY_COL)
        {
            String_View group = sv_from_parts(src.items + line + col, BYTES_BY_COL);
            if(col) da_append(dst, ' ');
            sb_append_as_hex(dst, group);
        }
        da_append_many(dst, "  ", 2);
        sb_append_ascii(dst, sv_from_parts(src.items + line, BYTES_BY_LINE));
        da_append(dst, '\n');
    }

    if(src.size == line) return;
    sb_append_uint(dst, line, 16, 8);
    da_append_many(dst, ": ", 2);

    size_t col;
    for(col = 0; col + BYTES_BY_COL <= src.size - line;
        col+=BYTES_BY_COL)
    {
        String_View group = sv_from_parts(src.items + line + col, BYTES_BY_COL);
        if(col) da_append(dst, ' ');
        sb_append_as_hex(dst, group);
    }

    if(line + col != src.size) {
        String_View group = sv_from_parts(src.items + line + col, src.size - line - col);
        if(col) da_append(dst, ' ');
        sb_append_as_hex(dst, group);
    }

    size_t padding = BYTES_BY_LINE - (src.size - line);
    padding = 2*padding + (padding / BYTES_BY_COL);
    // padding++;

    for(size_t i = 0; i < padding; i++) {
        da_append(dst, ' ');
    }

    da_append_many(dst, "  ", 2);
    sb_append_ascii(dst, sv_from_parts(src.items + line, src.size - line));
    da_append(dst, '\n');
}

#endif