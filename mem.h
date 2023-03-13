/* mem.h is the public header for memory functions
   The definitions here are shared with the user (bench) during
   completion of HW6, CSE374 22WI
*/

#ifndef MEM_H
#define MEM_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "mem_impl.h"

#define CHUNK 8192
#define BUFFER 128
#define MEM_HEADER_SIZE sizeof(uintptr_t) + sizeof(freeNode*)

/* Return a pointer to a new block of storage with at least 'size' bytes space.
   The 'size' value must be positive.  If it is not, or if there is another
   problem allocating the memory the function should return NULL. */
void* getmem(uintptr_t size);

/* Return the block of storage at location p to the pool of available free
   storage. The pointer value p must be one that was obtained as the result
   of a call to getmem. If p is NULL, then the call to freemem has no effect
   and returns immediately. */
void freemem(void* p);

/* Store statistics about the current state of the memory manager in the
   three integer variables whose addresses are given as arguments. The
   information stored should be as follows:
   total_size: total amount of storage in bytes acquired by the memory manager
   total_free: the total amount of storage in bytes that is currently stored
   on the free list.
   n_free_blocks: the total number of individual blocks currently stored
   on the free list. */
void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free,
                   uintptr_t* n_free_blocks);

/* Print a formatted listing on file f showing the blocks on the free list. */
void print_heap(FILE * f);

/* Helper function to insert blocks into freelist in order of   memory address                                                       
*/
void insert(freeNode* node);
/* Function to split block according to requested size, changing source
   block to requested size and creating new adjacent block of remaining difference
   between original block size and request.
 */
freeNode* split(freeNode* node, uintptr_t size);
/*
Helper function used to free a freeNode
*/
void free_node(freeNode* node);

/*
 * Fix to 16
 * Takes in a size ptr and fixs it to be divisible by 16
 * Returns the ptr
*/
uintptr_t fixto16(uintptr_t size);



#endif
