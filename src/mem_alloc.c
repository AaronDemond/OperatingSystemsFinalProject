#include "logger.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  unsigned int start;
  unsigned int capacity;
  bool is_free;
  unsigned int pid;
} mem_block;

typedef struct {
  mem_block *data;
  unsigned int max_cap;  // allocated array slots
  unsigned int curr_cap; // used array slots
  unsigned int total_mem;
} mblock_array;

// Input helpers (fgets-based to avoid scanf buffer issues)

int read_uint(const char *prompt, unsigned int *out) {
  char buffer[64];
  printf("%s", prompt);
  if (!fgets(buffer, sizeof(buffer), stdin))
    return 0;
  char *end;
  unsigned long val = strtoul(buffer, &end, 10);
  if (end == buffer || (*end != '\n' && *end != '\0')) {
    printf("Invalid input.\n");
    return 0;
  }
  *out = (unsigned int)val;
  return 1;
}

int read_int(const char *prompt, int *out) {
  char buf[64];
  printf("%s", prompt);
  if (!fgets(buf, sizeof(buf), stdin))
    return 0;
  char *end;
  long val = strtol(buf, &end, 10);
  if (end == buf || (*end != '\n' && *end != '\0')) {
    printf("Invalid input.\n");
    return 0;
  }
  *out = (int)val;
  return 1;
}

// Dynamic array operations

void mba_init(mblock_array *arr, unsigned int cap, unsigned int total_mem) {
  arr->curr_cap = 0;
  arr->max_cap = cap;
  arr->total_mem = total_mem;
  arr->data = malloc(cap * sizeof(mem_block));
}

void mba_realloc(mblock_array *arr) {
  arr->max_cap *= 2;
  arr->data = realloc(arr->data, sizeof(mem_block) * arr->max_cap);
}

void mba_insert(mblock_array *arr, unsigned int index, mem_block block) {
  if (arr->curr_cap == arr->max_cap)
    mba_realloc(arr);
  for (unsigned int i = arr->curr_cap; i > index; i--)
    arr->data[i] = arr->data[i - 1];
  arr->data[index] = block;
  arr->curr_cap++;
}

// Core operations

void mba_compact(mblock_array *arr) {
  unsigned int curr_start = 0;
  unsigned int allocated = 0;
  // slide all allocated blocks to the front
  for (unsigned int i = 0; i < arr->curr_cap; i++) {
    if (!arr->data[i].is_free) {
      arr->data[allocated] = arr->data[i];
      arr->data[allocated].start = curr_start;
      curr_start += arr->data[allocated].capacity;
      allocated++;
    }
  }
  // one free block at the end with all remaining space
  arr->data[allocated] = (mem_block){.start = curr_start,
                                     .capacity = arr->total_mem - curr_start,
                                     .is_free = true,
                                     .pid = 0};
  arr->curr_cap = allocated + 1;
  LOG("Memory compaction executed");
}

void mba_push(mblock_array *arr, unsigned int size, unsigned int pid) {
  if (size == 0 || size > arr->total_mem) {
    printf("Invalid allocation size.\n");
    return;
  }
  LOGF("Allocation request PID=%u SIZE=%u", pid, size);

  mem_block *blocks = arr->data;
  int loc = -1;

  // first-fit search
  for (unsigned int i = 0; i < arr->curr_cap; i++) {
    if (blocks[i].is_free && blocks[i].capacity >= size) {
      loc = i;
      break;
    }
  }

  if (loc != -1) {
    blocks[loc].is_free = false;
    blocks[loc].pid = pid;
    // split if leftover space
    if (blocks[loc].capacity != size) {
      if (arr->curr_cap + 1 == arr->max_cap) {
        mba_realloc(arr);
        blocks = arr->data;
      }
      mem_block remainder = {.start = blocks[loc].start + size,
                             .capacity = blocks[loc].capacity - size,
                             .is_free = true,
                             .pid = 0};
      blocks[loc].capacity = size;
      for (unsigned int i = arr->curr_cap; i > (unsigned int)loc + 1; i--)
        blocks[i] = blocks[i - 1];
      blocks[loc + 1] = remainder;
      arr->curr_cap++;
    }
  } else {
    // compact if enough total free space exists
    unsigned int total_free = 0;
    for (unsigned int i = 0; i < arr->curr_cap; i++) {
      if (arr->data[i].is_free)
        total_free += arr->data[i].capacity;
    }
    if (total_free >= size) {
      LOG("Fragmentation detected — performing compaction");
      mba_compact(arr);
      mba_push(arr, size, pid);
    } else {
      printf("Not enough memory.\n");
      LOG("Allocation failed — insufficient memory");
    }
  }
}

void mba_dealloc(mblock_array *arr, unsigned int pid) {
  LOGF("Deallocation request PID=%u", pid);
  bool found = false;
  for (unsigned int i = 0; i < arr->curr_cap; i++) {
    if (!arr->data[i].is_free && arr->data[i].pid == pid) {
      arr->data[i].pid = 0;
      arr->data[i].is_free = true;
      found = true;

      // merge with next if free
      if (i + 1 < arr->curr_cap && arr->data[i + 1].is_free) {
        arr->data[i].capacity += arr->data[i + 1].capacity;
        for (unsigned int j = i + 1; j < arr->curr_cap - 1; j++)
          arr->data[j] = arr->data[j + 1];
        arr->curr_cap--;
      }
      // merge with previous if free
      if (i > 0 && arr->data[i - 1].is_free) {
        arr->data[i - 1].capacity += arr->data[i].capacity;
        for (unsigned int j = i; j < arr->curr_cap - 1; j++)
          arr->data[j] = arr->data[j + 1];
        arr->curr_cap--;
        i--;
      }
    }
  }
  if (!found)
    printf("Process %u not found.\n", pid);
}

void mba_print(mblock_array *arr) {
  LOG("Displaying memory map");
  printf("\n%-10s %-10s %-10s\n", "Address", "Size", "Status");
  printf("------------------------------\n");
  for (unsigned int i = 0; i < arr->curr_cap; i++) {
    mem_block b = arr->data[i];
    if (b.is_free)
      printf("%-10u %-10u Free\n", b.start, b.capacity);
    else
      printf("%-10u %-10u Process %u\n", b.start, b.capacity, b.pid);
  }
  printf("\n");
}

void mba_free(mblock_array *arr) { free(arr->data); }

int main(void) {
  unsigned int total_mem = 0;
  while (!read_uint("Enter total memory size: ", &total_mem) ||
         total_mem == 0) {
    if (total_mem == 0)
      printf("Must be greater than 0.\n");
  }
  LOGF("Total memory entered: %u", total_mem);

  mblock_array arr;
  mba_init(&arr, 8, total_mem);

  // entire memory starts as one allocated "system" block (pid 0)
  arr.data[0] = (mem_block){
      .start = 0, .capacity = total_mem, .is_free = false, .pid = 0};
  arr.curr_cap = 1;

  unsigned int num_holes;
  while (!read_uint("Enter number of initial holes: ", &num_holes)) {
  }
  LOGF("Number of holes entered: %u", num_holes);

  for (unsigned int i = 0; i < num_holes; i++) {
    unsigned int start, size;
    while (!read_uint("Hole start: ", &start)) {
    }
    while (!read_uint("Hole size: ", &size)) {
    }

    // bounds check
    if (start >= total_mem || size == 0 || start + size > total_mem) {
      printf("Hole exceeds memory bounds.\n");
      i--;
      continue;
    }

    // find the block containing 'start' and validate placement
    bool placed = false;
    for (unsigned int j = 0; j < arr.curr_cap; j++) {
      mem_block *b = &arr.data[j];
      if (start >= b->start && start < b->start + b->capacity) {
        if (b->is_free) {
          printf("Hole overlaps an existing free region.\n");
          break;
        }
        if (start + size > b->start + b->capacity) {
          printf("Hole spans multiple blocks.\n");
          break;
        }

        unsigned int before_size = start - b->start;
        unsigned int after_start = start + size;
        unsigned int after_size = (b->start + b->capacity) - after_start;

        mem_block hole = {
            .start = start, .capacity = size, .is_free = true, .pid = 0};

        if (before_size > 0) {
          b->capacity = before_size;
          mba_insert(&arr, j + 1, hole);
          if (after_size > 0) {
            mem_block after = {.start = after_start,
                               .capacity = after_size,
                               .is_free = false,
                               .pid = 0};
            mba_insert(&arr, j + 2, after);
          }
        } else {
          *b = hole;
          if (after_size > 0) {
            mem_block after = {.start = after_start,
                               .capacity = after_size,
                               .is_free = false,
                               .pid = 0};
            mba_insert(&arr, j + 1, after);
          }
        }
        placed = true;
        break;
      }
    }
    if (!placed) {
      printf("Invalid hole, try again.\n");
      i--;
    } else {
      LOGF("Hole defined start=%u size=%u", start, size);
    }
  }

  printf("\nInitial memory layout:");
  mba_print(&arr);

  int choice;
  do {
    printf("--- Memory Manager ---\n");
    printf("1. Allocate\n");
    printf("2. Deallocate\n");
    printf("3. Display Memory Map\n");
    printf("4. Compact\n");
    printf("5. Exit\n");
    if (!read_int("Choice: ", &choice))
      continue;
    LOGF("Memory menu choice: %d", choice);

    switch (choice) {
    case 1: {
      unsigned int pid, size;
      if (!read_uint("Process ID: ", &pid))
        break;
      if (!read_uint("Size: ", &size))
        break;
      mba_push(&arr, size, pid);
      break;
    }
    case 2: {
      unsigned int pid;
      if (!read_uint("Process ID: ", &pid))
        break;
      mba_dealloc(&arr, pid);
      break;
    }
    case 3:
      mba_print(&arr);
      break;
    case 4:
      mba_compact(&arr);
      printf("Memory compacted.\n");
      mba_print(&arr);
      break;
    case 5:
      printf("Exiting.\n");
      LOG("Memory manager exited");
      break;
    default:
      printf("Invalid choice.\n");
    }
  } while (choice != 5);

  mba_free(&arr);
  return 0;
}
