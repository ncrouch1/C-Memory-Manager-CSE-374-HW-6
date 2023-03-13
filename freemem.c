/* memory.c
   implements freemem (free) function for memory system
   CSE 374 HW6 - U of WA - Chase Vara, Noah Crouch
*/

#include <assert.h>
#include "mem.h"
#include "mem_impl.h"

/* This code will be filled in by the students.  Your primary job is to
   implement getmem and freemem functions.  */

/* initialize global variables */
// Predeclared helper function
freeNode* combine(uintptr_t memstart, uintptr_t memend, uintptr_t memsize);
/* Function to free memory block, returning it to freelist.
    Processes freelist to combine any adjacent blocks
 */
void freemem(void* p) {
   // if p is null return
   if (!p) return;
   // get the start of the memory address, the end of the memory address
   // and compute the size
   uintptr_t memstart = (uintptr_t) p;
   uintptr_t memend = memstart + sizeof(p);
   uintptr_t memsize = memend - memstart;
   // Add the size to total free blocks
   total_free_blocks += memsize;
   // combine the memory if possible and insert it if it was combined
   freeNode* combined = combine(memstart, memend, memsize);
   insert(combined);
}

// Combine helper function, takes in a memory address start, end, and size
// and attempts to find an adjacent node to combine with
freeNode* combine(uintptr_t memstart, uintptr_t memend, uintptr_t memsize) {
   // if the list is null skip and return null
   if (freelist != (freeNode*) NULL) {
      // get the list
      freeNode* current = freelist;
      // aquire the start and end of the node
      uintptr_t currstart = (uintptr_t) current - MEM_HEADER_SIZE;
      uintptr_t currend = currstart + current->size + NODESIZE;
      // if the node end is adjacent to the freed mem combine into that node
      if (currend + 1 == memstart) {
         current->size += memsize + MEM_HEADER_SIZE;
         freeNode* newcurr = current;
         current = current->next;
         newcurr->next = (freeNode*) NULL;
         return newcurr;
      // else if the current node start is adjacent to the end of the freed mem
      // make a new node calculate the size and return that
      } else if (currstart - 1 == memend) {
         freeNode* mem = (freeNode*) memstart;
         mem->size = current->size + memsize + MEM_HEADER_SIZE;
         current = current->next;
         return mem;
      }
      // Do the same as the front case here for the rest of the list
      while (current->next != (freeNode*) NULL) {
         currstart = (uintptr_t) current->next - MEM_HEADER_SIZE;
         currend = currstart + current->next->size + MEM_HEADER_SIZE;
         if (currend + 1 == memstart) {
            current->next->size += memsize + NODESIZE;
            freeNode* newcurr = current->next;
            current->next = current->next->next;
            newcurr->next = (freeNode*) NULL;
            return newcurr;
         } else if (currstart - 1 == memend) {
            freeNode* mem = (freeNode*) memstart;
            mem->size = current->next->size + memsize + MEM_HEADER_SIZE;
            current->next = current->next->next;
            return mem;
         }
         current = current->next;
      }
   }
   return (freeNode*) NULL;
}
