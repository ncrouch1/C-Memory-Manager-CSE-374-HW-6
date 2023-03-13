/* memory.c
   implements getmem (malloc) function for memory system
   CSE 374 HW6 - U of WA - Chase Vara, Noah Crouch
*/

#include <assert.h>
#include "mem.h"
#include "mem_impl.h"

/* This code will be filled in by the students.  Your primary job is to
   implement getmem and freemem functions.  */

// Declare package global vars here
freeNode* freelist;
uintptr_t totalmalloc = 0;
uintptr_t total_free_blocks = 0;
uintptr_t total_blocks = 0;

// Pointers for get mem stats function
uintptr_t* total_size = &totalmalloc;
uintptr_t* total_free = &total_free_blocks;
uintptr_t* n_free_blocks = &total_blocks;

// Primary function
void* getmem(uintptr_t size) {
  // If size is less than 0 for some reason, return NULL
  if (size <= 0) {
    return (void*) NULL;
  }
  // If the freelist is uninitiallized, initialize it here
  if (!freelist) {
    uintptr_t init_size = fixto16(CHUNK);
    freelist = (freeNode*) malloc(init_size);
    freelist->size = init_size - MEM_HEADER_SIZE;
    freelist->next = (freeNode*) NULL;
    totalmalloc += init_size;
    total_free_blocks += init_size;
  }

  // Format size to be aligned with the 16 byte size standard
  if (size < MINCHUNK) {
    size = MINCHUNK;
  }
  uintptr_t fixed_size = fixto16(size);
  
  if (freelist) {
    freeNode* current = freelist;
    // Front case, if the front of the list has enough mem to 
    // satisfy the current get mem call
    if (current->size >= fixed_size + BUFFER + MEM_HEADER_SIZE) {
      freeNode* remainder = split(current, fixed_size);
      void* allocated_mem = (void*) (((uintptr_t) current) + MEM_HEADER_SIZE);
      current = remainder;
      total_free_blocks -= (fixed_size + NODESIZE);
      return allocated_mem;
    }
    // middle and end case, if the front of the list doesn't have enough mem 
    // search the rest of the list
    while (current->next) {
      if (current->next->size >= fixed_size + BUFFER + MEM_HEADER_SIZE) {
        freeNode* remainder = split(current->next, fixed_size);
        void* allocated_mem = (void*) (((uintptr_t) current) + MEM_HEADER_SIZE);
        current->next = remainder;
        total_free_blocks -= (fixed_size + NODESIZE);
        return allocated_mem;
      }
      current = current->next;
    }
  }

  // No node in list can satisfy case:
  // If no freeNode in list has size big enough
  uintptr_t newnodesize = fixto16(size + 20000);
  freeNode* newnode = (freeNode*) malloc(newnodesize);
  newnode->size = newnodesize - MEM_HEADER_SIZE;
  newnode->next = (freeNode*) NULL;
  totalmalloc += newnodesize;
  total_free_blocks += newnodesize - fixed_size - NODESIZE;
  freeNode* remainder = split(newnode, fixed_size);
  insert(remainder);
  void* allocated_mem = (void*) (((uintptr_t) newnode) + MEM_HEADER_SIZE);
  return allocated_mem;
}





