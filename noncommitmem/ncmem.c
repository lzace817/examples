#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>

#define RESERVE_SIZE (1L*1024*1024*1024)
#define TEST_COUNT (32*1024)
#define CHUNK_SIZE (64L*1024)

void read_write_test(void *data)
{
    int *array = (int *)data;
    int status = 0;
    // comment bellow to segfault
    status = mprotect(data, 2*CHUNK_SIZE, PROT_WRITE | PROT_READ);
    assert(status == 0);

    for(int i = 0; i < TEST_COUNT; i++) {
        array[i] = i;
    }

    for(int i = 0; i < TEST_COUNT; i++) {
        assert(array[i] == i);
    }

    printf("memory_read_write test done!\n");

}

int main(void)
{
    void *data = mmap(0, RESERVE_SIZE,
                        PROT_NONE, /* Over-committing with: PROT_READ | PROT_WRITE, */
                        MAP_ANONYMOUS | MAP_PRIVATE | MAP_NORESERVE,
                        -1, 0);
    if (data == MAP_FAILED) {
        perror("ncmem");
        exit(1);
    }

    int status = mprotect(data, CHUNK_SIZE, PROT_WRITE | PROT_READ);
    if(status == -1) {
        perror("ncmem");
        exit(1);
    }

    printf("map start at: %p\n", data);

    printf("press ENTER to allocate and test\n");
    while (getchar() != '\n');

    read_write_test(data);

    printf("press ENTER to try give back");
    while (getchar() != '\n');

    status = mprotect(data + CHUNK_SIZE, CHUNK_SIZE, PROT_NONE);
    if(status == -1) {
        perror("ncmem");
        exit(1);
    }

    printf("\n\npress ENTER to release\n");
    while (getchar() != '\n');

    status = munmap(data, RESERVE_SIZE);
    if(status == -1) {
        perror("ncmem");
        exit(1);
    }

    printf("\n\npress ENTER to close application\n");
    while (getchar() != '\n');

    return 0;
}