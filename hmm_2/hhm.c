/* *******
 * @File                 : hhm.c
 * @Author               : Mohamed Bahget Hamam
 * @gmail                : mahamedhamam15@gmail.com
 *******
*/

/* ******** include section start *********** */

#include <stdint.h>                                         // includes the standard integer types header.
#include <stdio.h>                                          // includes the standard input/output header.
#include <string.h>                                         // includes the string manipulation functions.
#include <unistd.h>                                         // for sbrk().
#include <errno.h>                                          // for errno.
#include "hhm.h"                                            // includes the header file which has function declarations.

/* ******** Include section End *********** */

/* ******** macros section Start *********** */

#define HEAP_SIZE (1024 * 1024)                             // 1 MB heap size.
#define BLOCK_HEADER_SIZE sizeof(block_header)              // define the size of block header.

/* ******** macros section End *********** */

/* ******* Definition section start ******* */

typedef struct block_header                                 // structure to manage memory blocks.
{
    size_t size;                                            // the size of the block.
    int free;                                               // flag to indicate if the block is free or not.
    struct block_header *next;                              // pointer to the next block.
} block_header;

static char *heap_start = NULL ;                             // pointer to the start of the heap.
static char *heap_end = NULL ;                               // pointer to the end of the heap.
static char *current_brk = NULL ;                            // pointer to the current end of the heap.

static void init_heap()                                      // initializes the heap if not done already.
{
    if (heap_start == NULL)
    {
        heap_start = (char *)sbrk(HEAP_SIZE);                 // expand the heap by HEAP_SIZE using sbrk().
        if (heap_start == (void *)-1)                         // check for errors.
        {
            perror("sbrk error");
            _exit(1);                                         // terminate the program.
        }
        
        heap_end = heap_start + HEAP_SIZE;                    // set the end of the heap.
        current_brk = heap_start;                             // set the current end of the heap.
    }
}

static block_header* find_free_block(size_t size)            // finds a free block that fits the requested size.
{
    block_header *current = (block_header *)heap_start;       // start from the beginning of the heap.
    while (current && !(current->free && current->size >= size)) // iterate through blocks to find a suitable free block.
    {
        current = current->next;                             // move to the next block.
    }
    
    return current;                                          // return found block or NULL.
}

void *HmmAlloc(size_t size)                                  // allocates a block of memory.
{
    if (size == 0) return NULL;                              // if size = 0 return NULL.

    if (heap_start == NULL)
    {
        init_heap();                                         // initialize the heap.
    }

    size = (size + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1); // align the size to the nearest multiple of sizeof(size_t).

    block_header *block = find_free_block(size);             // find a free block.

    if (block)                                               // suitable block found?
    {
        /* block splitting */
        if (block->size > size + BLOCK_HEADER_SIZE)
        {
            block_header *new_block = (block_header *)((char *)block + BLOCK_HEADER_SIZE + size);
            new_block->size = block->size - size - BLOCK_HEADER_SIZE;
            new_block->free = 1;
            new_block->next = block->next;

            block->size = size;
            block->next = new_block;
        }
        block->free = 0;                                     // mark it as used.
        return (void *)(block + 1);                          // return pointer to the data part.
    }

    if (current_brk + size + BLOCK_HEADER_SIZE > heap_end)  // do you need more memory?
    {
        size_t expand_size = size + BLOCK_HEADER_SIZE;

        if (sbrk(expand_size) == (void *)-1)                 // expand using sbrk() and check for errors.
        {
            perror("sbrk error");
            return NULL;
        }
        heap_end += expand_size;                             // update the end of the heap.
    }

    block = (block_header *)current_brk;                     // allocate a new block.
    block->size = size;                                      // set the size of the block.
    block->free = 0;                                         // mark block as used.
    block->next = NULL;                                      // initialize the next block pointer.

    if (heap_start == (char *)block)
    {
        heap_start = (char *)block;
    } else 
    {
        block_header *current = (block_header *)heap_start;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = block;
    }

    current_brk += size + BLOCK_HEADER_SIZE;                 // update the current end of the heap.
    return (void *)(block + 1);                              // return pointer to the data part of the block.
}

void HmmFree(void *ptr)                                      // frees a block of memory.
{
    if (ptr == NULL) return;                                 // return if the pointer is NULL.

    block_header *block = (block_header *)ptr - 1;           // calculate the block header address.
    block->free = 1;                                         // mark the block as free.

    printf("freed block at address %p with size %zu \n", (void *)block , block->size);

    /* merge adjacent free blocks */
    block_header *current = (block_header *)heap_start;      // start from the beginning of the heap.
    while (current && current->next)                         // iterate through the blocks.
    {
        if (current->free && current->next->free)            // check if the current and next are free.
        {
            current->size += current->next->size + BLOCK_HEADER_SIZE;  // merge the current and the next block.
            current->next = current->next->next;             // update the next pointer.

            printf("Merged block at %p with next block\n", (void *)current);
        } else 
        {
            current = current->next;                         // move to the next block.
        }
    }
}

void *HmmRealloc(void *ptr, size_t size)                     // reallocates a block of memory.
{
    if (ptr == NULL) return HmmAlloc(size);                  // allocate a new block if ptr is NULL.

    block_header *old_block = (block_header *)ptr - 1;       // calculate the address of the old block header.

    size = (size + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1);

    if (size <= old_block->size) return ptr;                 // return the old pointer if the new size is less than or equal to the old size.

    void *new_ptr = HmmAlloc(size);                          // allocate a new block.
    if (new_ptr == NULL) return NULL;                        // return NULL if allocation fails.

    memcpy(new_ptr, ptr, old_block->size);                   // copy data from the old block to the new block.
    HmmFree(ptr);                                            // free the old block.

    return new_ptr;                                          // return the new pointer.
}

void *HmmCalloc(size_t num, size_t size)                     // allocates and zero-initializes memory.
{
    void *ptr = HmmAlloc(num * size);                        // allocate memory.
    if (ptr)                                                 // allocation success? Zero-initialize memory.
    {
        memset(ptr, 0, num * size);                          // set the allocated memory to zero.
    }

    return ptr;                                              // return the pointer.
}

/* Wrapping libc functions */

void *malloc(size_t size)                                    // Wraps HmmAlloc to provide a standard malloc interface.
{
    return HmmAlloc(size);
}

void free(void *ptr)                                         // Wraps HmmFree to provide a standard free interface.
{
    HmmFree(ptr);
}

void *realloc(void *ptr, size_t size)                        // Wraps HmmRealloc to provide a standard realloc interface.
{
    return HmmRealloc(ptr, size);
}

void *calloc(size_t num, size_t size)                        // Wraps HmmCalloc to provide a standard calloc interface.
{
    return HmmCalloc(num, size);
}

/* *********** Definition section End ******** */

/* **********************************************
    User                Date               Time
   ***********************************************
   Mohamed Hamam       20 Aug 2024        02:54 PM
   Mohamed Hamam       27 aug 2024        05:54 PM
*/
