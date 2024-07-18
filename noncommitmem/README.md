# NON_COMMITED memory on linux

Example/exploration on allocating memory with is not commited imediatelly.

The objetive is to be able to grow an allocation without the need to move the data to a new location.

## libs

- sys/mman.h

## Reserve the virtual memory region

existe o recurso `Over-committing Memory` que pode ser usado.
Porém a maneira exemplificada aqui será:

``` C
void *data = mmap(0, RESERVE_SIZE,
                    PROT_NONE,
                    MAP_ANONYMOUS | MAP_PRIVATE,
                    -1, 0);
```

`PROT_NONE` faz com que o endereço de memória virtual seja reservado
porém a memoria fisica não é consumida, porquê a região de memória está inacessível.


## Make more memory available

``` C
int status = mprotect(data, new_commited_size,
                        PROT_WRITE | PROT_READ);
```

ao desejar extender a memória disponível dentro da região reservada, conceda permissão de leitura e escrita para um espaço dentro dessa região.

## how to give back memory?

on windows, `VirtualAlloc` with `MEM_COMMIT`, `MEM_RESERVE` and etc, can be used to reserve virtual memory adress. But I'm not sure on how to do some of the corresponding stuff in linux.

## status

`status#.txt` stores the contents of `/proc/[pid]/status`
diffing them is very instructive.

`pmap -x <PID>` to se the virtual to resident comparison

### VmData
the VmData field represents the size of the data segment of a process. This includes the heap, any dynamically allocated memory (using malloc, for example), and memory mapped files. Essentially, it is the portion of the process's virtual memory space used for the process's data.

### VmRss
Resident set size (size in kB of pages the process has in real memory).