/* mem_utils.c utilities to support memory management system.
   HW6, CSE 374 - U of WA - Chase Vara, Noah Crouch
*/

#include <inttypes.h>
#include "mem.h"
#include "mem_impl.h"

uintptr_t countfree();
// Given helper function that asserts that the heap is implemented correctly
void check_heap() {
  if (!freelist) return;
  freeNode* currentNode = freelist;
  uintptr_t minsize = currentNode->size;

  while (currentNode != NULL) {
    if (currentNode->size < minsize) {
      minsize = currentNode->size;
    }
    if (currentNode->next != NULL) {
      assert((uintptr_t)currentNode <(uintptr_t)(currentNode->next));
      assert((uintptr_t)currentNode + currentNode->size + NODESIZE
              <(uintptr_t)(currentNode->next));
    }
    currentNode = currentNode->next;
  }
  // go through free list and check for all the things
  if (minsize == 0) print_heap(stdout);
  assert(minsize >= MINCHUNK);
}

// Insert helper function, takes the freenode and inserts the new node
// where it belongs with the address
void insert(freeNode* ins) {
  if (freelist != (freeNode*) NULL && ins != (freeNode*) NULL) {
    // Get current as the freelist reference
    // Get the address of the inserting node
    freeNode* current = freelist;
    uintptr_t ins_address = (uintptr_t) ins;
    
    // If the address of the inserting node is before
    // the front of the freelist node, insert the node 
    // at the front of the freelist
    if (ins_address < (uintptr_t) current) {
      ins->next = current;
      freelist = ins;
    }
    while (current->next != (freeNode*) NULL) {
      if (ins_address < (uintptr_t) current->next) {
        ins->next = current->next;
        current->next = current;
        return;
      }
      current = current->next;
    }

    // If the function has not exitted by now, that means that the address
    // of the inserting node is further than all the other nodes, so we will
    // insert it at the end of the list
    current->next = ins;
  } else if (freelist == (freeNode*) NULL && ins != (freeNode*) NULL) freelist = ins;
}

// Split helper function, takes in a node and breaks off a chunk to allocate
freeNode* split(freeNode* node, uintptr_t size) {
  uintptr_t newsize = node->size - size - MEM_HEADER_SIZE;
  uintptr_t newadd = ((uintptr_t) node) + size;

  freeNode* remainder = (freeNode*) newadd;
  remainder->size = newsize;
  remainder->next = node->next;

  node->size = size;
  node->next = (freeNode*) NULL;

  return remainder;
}

void free_node(freeNode* node) {
  free(node->next);
  free(node);
}

uintptr_t fixto16(uintptr_t size) {
  if (size % MINCHUNK == 0) {
    return size;
  } else {
    return size + (MINCHUNK - size % MINCHUNK);
  }
}

// client function to store memory manager stats in given parameters                                                                        
void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free,
                   uintptr_t* n_free_blocks) {
  *total_size = totalmalloc;
  *total_free = total_free_blocks;
  *n_free_blocks = countfree();
}

/* Function to print list of memory blocks on heap:
   <block address> <blocksize> <next block adress>
 */
void print_heap(FILE *f) {
  printf("Printing the heap\n");
  freeNode* currentNode = freelist;
  while (currentNode !=NULL) {
    fprintf(f, "%" PRIxPTR, (uintptr_t)currentNode);
    fprintf(f, ", size: %" PRIdPTR, currentNode->size);
    fprintf(f, ", next: %" PRIxPTR, (uintptr_t)currentNode->next);
    fprintf(f, "\n");
    currentNode = currentNode->next;
  }
}

// helper function to return number of memory blocks contained in freelist
uintptr_t countfree() {
  uintptr_t count = 0;
  if (freelist) {
    freeNode* current = freelist;
    while (current) {
      count++;
      current = current->next;
    }
  }
  return count;
}
