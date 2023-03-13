/* bench.c is the benchmarking /test program for mem memory management
   bench.c is implemented as part of HW6, CSE374 22WI

   Copyright 2022 Noah Crouch, Chase Vara
                  University of Washington
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <inttypes.h>
#include "mem.h"
#include "mem_impl.h"




// Predeclared helper functions
void print_status(int i, uintptr_t NTRIALS, clock_t start);
void print_stats(clock_t start, clock_t end,
                 uintptr_t total_allocated,
                 uintptr_t total_free,
                 uintptr_t total_blocks);
void fill_mem(void* ptr, uintptr_t size);

/* Synopsis:   bench (main)
   [ntrials] (10000) getmem + freemem calls
   [pctget] (50) % of calls that are get mem
   [pctlarge] (10) % of calls requesting more memory than lower limit
   [small_limit] (200) largest size in bytes of small block
   [large_limit] (20000) largest size in byes of large block
   [random_seed] (time) initial seed for randn
*/
int main(int argc, char** argv) {
  // Initialize the parameters
  int NTRIALS;
  int PCTGET;
  int PCTLARGE;
  int SMALL_L;
  int LARGE_L;

  (argc > 1) ? (NTRIALS = atoi(argv[1])) : (NTRIALS = 10000);
  (argc > 2) ? (PCTGET = atoi(argv[2])) : (PCTGET = 50);
  (argc > 3) ? (PCTLARGE = atoi(argv[3])) : (PCTLARGE = 10);
  (argc > 4) ? (SMALL_L = atoi(argv[4])) : (SMALL_L = 200);
  (argc > 5) ? (LARGE_L = atoi(argv[5])) : (LARGE_L = 20000);

  // initialize random number gen.
  (argc > 6) ? srand(atoi(argv[6])) : srand(time(NULL));

  printf("Running bench for %d trials, %d%% getmem calls.\n", NTRIALS, PCTGET);

  void* blocks[NTRIALS];  // upperbound block storage
  //int ntrials = 0, nblocks = 0;
  // start time and initialize indexer for blocks.
  clock_t start = clock();
  int last_get = -1;

  // perform NTRIALS mem operations

  /* Students will fill in this section according to the assignment
     specs.*/
  for (int i = 0; i < NTRIALS; i++) {
    // Print status in increments of 10
    print_status(i, NTRIALS, start);
    // get a random num
    int random_num = rand() % 100;
    // check the heap
    check_heap();
    // randomly choose get mem or free mem
    if (random_num + 1 < PCTGET) {
      // if get mem, randomly choose small or large
      uintptr_t size;
      if (random_num + 1 <= PCTLARGE) {
        size = (uintptr_t) rand() % LARGE_L;
      } else {
        size = (uintptr_t) rand() % SMALL_L;
      }
      // increment the alloc index
      last_get++;
      // allocate mem
      blocks[last_get] = getmem(size);
    } else {
      // only call free mem if indexer is above -1
      if(last_get >= 0) {
        // only decrement indexer if its not -1
        if(last_get != -1) {
          last_get--;
        }
        // Free the last allocated memory
        freemem(blocks[last_get]);
      }
    }
    // print stats then check again
    get_mem_stats(total_size, total_free, n_free_blocks);
    check_heap();
  }
  // Get the end time and print the stats
  clock_t end = clock();
  printf("Finished testing\n");
  print_stats(start, end, totalmalloc, total_free_blocks, total_blocks);
  printf("Checking heap one last time\n");
  check_heap();
  return EXIT_SUCCESS;
}

// Helper function that prints the stats of the current interval
void print_status(int i, uintptr_t NTRIALS, clock_t start) {
  if (NTRIALS/10 == 0) return;
  if (i % (NTRIALS / 10) == 0 && i != 0) {
      clock_t time = clock();
      printf("%lu percent done\n", i / (NTRIALS / 10) * 10);
      print_stats(start, time, totalmalloc, total_free_blocks, total_blocks);
  }
}



/* print_stats is a helper function that prints performance
   information about the memory system.  Requires a clock_t start
   time, and prints elapsed time plus results from get_mem_stats()
*/
void print_stats(clock_t start, clock_t end,
                 uintptr_t total_allocated,
                 uintptr_t total_free,
                 uintptr_t total_blocks) {
  printf("Total Allocated: %" PRIdPTR, total_allocated);
  printf("\n");
  printf("Total Free: %" PRIdPTR, total_free);
  printf("\n");
  printf("Total Blocks: %" PRIdPTR, total_blocks);
  printf("\n");
  if (total_blocks != 0) {
    printf("Average of %lu", total_free / total_blocks);
    printf(" bytes per free block\n");
  } else {
    printf("Average of 0 bytes per free block\n");
  }
  double time = (double)end - (double)start;
  printf("Time passed: %.2f\n", time);
}

/* fill_mem is a helper function that takes in an address and the
   size of the memory allocation at that address.  It should fill
   the first 16 bytes (or up to size) with hexedecimal values.*/
void fill_mem(void* ptr, uintptr_t size) {
  int bound = 16;
  if (size < 16) {
    bound = size;
  }
  uintptr_t* current = (uintptr_t*) ptr;
  for (int i = 0; i < bound; i++) {
    *current = 0xFE;
    current = current + 1;
  }
}


