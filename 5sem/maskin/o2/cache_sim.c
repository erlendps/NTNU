#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  dm,
  fa
} cache_map_t;
typedef enum {
  uc,
  sc
} cache_org_t;
typedef enum {
  instruction,
  data
} access_t;

typedef struct
{
  uint32_t address;
  access_t accesstype;
} mem_access_t;

typedef struct
{
  uint64_t accesses;
  uint64_t hits;
} cache_stat_t;

/* Struct for a single cache_entry. I have not cared about the data block
 */
typedef struct {
  bool valid;
  u_int32_t tag;
} cache_entry_t;

// DECLARE CACHES AND COUNTERS FOR THE STATS HERE

uint32_t cache_size;
uint32_t block_size = 64;
cache_map_t cache_mapping;
cache_org_t cache_org;

// Set up fifo-queue variables
// will point to the next-to-be-replaced cache-entry, if we have a fa mapping
uint32_t fifo_instruction_index = 0;
uint32_t fifo_data_index = 0;

// USE THIS FOR YOUR CACHE STATISTICS
cache_stat_t cache_statistics;

/* Reads a memory access from the trace file and returns
 * 1) access type (instruction or data access
 * 2) memory address
 */
mem_access_t read_transaction(FILE *ptr_file) {
  char type;
  mem_access_t access;

  if (fscanf(ptr_file, "%c %x\n", &type, &access.address) == 2) {
    if (type != 'I' && type != 'D') {
      printf("Unkown access type\n");
      exit(0);
    }
    access.accesstype = (type == 'I') ? instruction : data;
    return access;
  }

  /* If there are no more entries in the file,
   * return an address 0 that will terminate the infinite loop in main
   */
  access.address = 0;
  return access;
}

/* Increments the stats counter by incrementing the number of accesses
 * and if there is a hit, increment the number of hits.
 */
void increment_stats(bool hit) {
  cache_statistics.accesses++;
  if (hit == 1) {
    cache_statistics.hits++;
  }
}

/* Writes to the "cache". Takes in a pointer to the cache entry to overwrite,
 * and a tag that should be the new identifier for this entry.
 */
void write_to_cache(cache_entry_t *entry_pointer, uint32_t tag) {
  entry_pointer->valid = true;
  entry_pointer->tag = tag;
}

void main(int argc, char **argv) {
  // Reset statistics:
  memset(&cache_statistics, 0, sizeof(cache_stat_t));

  /* Read command-line parameters and initialize:
   * cache_size, cache_mapping and cache_org variables
   */
  if (argc != 4) { /* argc should be 2 for correct execution */
    printf(
        "Usage: ./cache_sim [cache size: 128-4096] [cache mapping: dm|fa] "
        "[cache organization: uc|sc]\n");
    exit(0);
  } else {
    /* Set cache size */
    cache_size = atoi(argv[1]);

    /* Set Cache Mapping */
    if (strcmp(argv[2], "dm") == 0) {
      cache_mapping = dm;
    } else if (strcmp(argv[2], "fa") == 0) {
      cache_mapping = fa;
    } else {
      printf("Unknown cache mapping\n");
      exit(0);
    }

    /* Set Cache Organization */
    if (strcmp(argv[3], "uc") == 0) {
      cache_org = uc;
    } else if (strcmp(argv[3], "sc") == 0) {
      cache_org = sc;
    } else {
      printf("Unknown cache organization\n");
      exit(0);
    }
  }

  // set the index bits, decide wheter cache size is valid
  int index_bits;  // number of index bits
  if (cache_mapping == dm) {
    switch (cache_size) {
      case 128:
        index_bits = 1;
        break;
      case 256:
        index_bits = 2;
        break;
      case 512:
        index_bits = 3;
        break;
      case 1024:
        index_bits = 4;
        break;
      case 2048:
        index_bits = 5;
        break;
      case 4096:
        index_bits = 6;
        break;
      default:
        printf("\nPlease input a valid cache size.\n");
        exit(0);
    }
    // if its a split cache, remove 1 from the index since the cache size is "halved".
    // This will awlays be valid, if we have a 128 byte cache (meaning 2 entries), we
    // need 1 bit for indexing. If the cache is split aswell (and fa), it will work in
    // the same way as a fa cache with only one entry.
    if (cache_org == sc) {
      index_bits--;
    }
  } else {
    // fa case, no need for indexing bits
    index_bits = 0;
  }

  // calculate number of cache blocks, and create new array with that length
  uint32_t num_of_entries = cache_size / block_size;
  cache_entry_t cache_entries[num_of_entries];

  // fill the array with placeholder values, i.e tag 0 (invalid) and valid bit set to 0
  cache_entry_t placeholder;
  for (int i = 0; i < num_of_entries; i++) {
    placeholder.tag = 0;
    placeholder.valid = false;
    cache_entries[i] = placeholder;
  }

  // set up pointers for instruction and data.
  // The instruction pointer will always point to the start of the array, in both the
  // unified and split cache cases. If the cache is unified, the data pointer can be
  // the same as the instruction pointer, since it uses the same cache. If however, the
  // cache is split, we set the data pointer to start at the halway point of the cache_entries
  // array. This creates a virtual split, and allows for manipulating the "same" array in
  // all cases.
  cache_entry_t *instruction_pointer = cache_entries;
  cache_entry_t *data_pointer;
  if (cache_org == uc) {
    data_pointer = cache_entries;
  } else {
    data_pointer = cache_entries + (num_of_entries / 2);
  }

  /* Open the file mem_trace.txt to read memory accesses */
  FILE *ptr_file;
  ptr_file = fopen("mem_trace.txt", "r");
  if (!ptr_file) {
    printf("Unable to open the trace file\n");
    exit(1);
  }

  mem_access_t access;

  // create mask for fetching tag bits and index bits.
  // these "isolates" the bits we need. We do however need to
  // right-shift the result back after we have ANDed the address
  // with the index and tag masks.
  uint32_t index_mask = ((1 << index_bits) - 1) << 6;
  uint32_t tag_mask = ((1 << (26 - index_bits)) - 1) << (6 + index_bits);

  // variables used in the cache main loop
  uint32_t tag;
  uint32_t index;
  uint32_t offset;  // offset from the cache_entry-pointer.
  bool hit;

  // set up search length
  // the search length indicates how long the cache should search for an entry.
  // If its a dm cache we only search once, while in the fa case, it depends on
  // the number of entries.
  uint32_t search_length;
  if (cache_mapping == dm) {
    search_length = 1;
  } else {
    search_length = (cache_org == uc) ? num_of_entries : num_of_entries / 2;
  }
  // current_pointer points to the current cache_entry in cache_entries
  cache_entry_t *current_pointer;
  while (1) {
    // reset used variables
    hit = false;
    offset = 0;

    access = read_transaction(ptr_file);
    // If no transactions left, break out of loop
    if (access.address == 0)
      break;

    // isolate the tag bits and right shift so we get the correct number of bits.
    // should be 32 - (6 + index_bits). We shift with 6 because that is reserved for the
    // data offset
    tag = (access.address & tag_mask) >> (6 + index_bits);
    // in the case of fa cache, this will always be 0, since the mask will be 0.
    index = (access.address & index_mask) >> 6;

    // determine type of access, and set the base address to the corresponding access
    if (access.accesstype == instruction) {
      current_pointer = instruction_pointer;
    } else {
      current_pointer = data_pointer;
    }
    do {
      // increment pointer
      current_pointer += index + offset;

      // check if its a cache hit
      if (current_pointer->valid == true && current_pointer->tag == tag) {
        increment_stats(true);
        hit = true;
        break;
      }
      // increment offset in pointer
      offset++;
    } while (offset < search_length);
    // the loop will terminate after one iteration if its a dm cache or a 128 byte fa sc

    // if we have a cache hit, continue at the next iteration
    if (hit) {
      continue;
    }
    // cache miss
    increment_stats(false);

    // replacement of cache
    // if we have a dm cache, the current_pointer simply points to the cache_block to
    // be replaced, and we don't need to modify current_pointer.
    // If however the mapping is fa, the fifo-indexes determine which block
    // should be replaced. In the case of a uc, we only use the instruction index.
    // For sc, we use both, and set the current_pointer to the cache base address
    // plus the index (offset). Since its fifo, we can just "go in a circle", so we
    // don't need to implement a queue.
    if (cache_mapping == fa) {
      if (cache_org == uc || (cache_org == sc && access.accesstype == instruction)) {
        current_pointer = instruction_pointer + fifo_instruction_index;
        fifo_instruction_index = (fifo_instruction_index + 1) % search_length;
      } else {
        current_pointer = data_pointer + fifo_data_index;
        fifo_data_index = (fifo_data_index + 1) % search_length;
      }
    }
    // finally, write to cache
    write_to_cache(current_pointer, tag);
  }

  /* Print the statistics */
  // DO NOT CHANGE THE FOLLOWING LINES!
  printf("\nCache Statistics\n");
  printf("-----------------\n\n");
  printf("Accesses: %ld\n", cache_statistics.accesses);
  printf("Hits:     %ld\n", cache_statistics.hits);
  printf("Hit Rate: %.4f\n",
         (double)cache_statistics.hits / cache_statistics.accesses);
  // DO NOT CHANGE UNTIL HERE
  // Close the trace file
  fclose(ptr_file);
}
