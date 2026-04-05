#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

typedef struct {
  unsigned int start;
  unsigned int capacity;
  bool is_free;
  unsigned int pid;
} mem_block;

typedef struct {
  mem_block *data;
  unsigned int max_cap;
  unsigned int curr_cap;
  unsigned int total_mem;
} mblock_array;

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

void mba_compact(mblock_array *arr) {

  unsigned int curr_start = 0;
  unsigned int allocated = 0;
  for (int i = 0; i < arr->curr_cap; i++) {
    if (!arr->data[i].is_free) {
      arr->data[allocated] = arr->data[i];
      arr->data[allocated].start = curr_start;
      curr_start += arr->data[allocated].capacity;
      allocated++;
    }
  }
  arr->data[allocated] = (mem_block){.start = curr_start,
                                     .capacity = arr->total_mem - curr_start,
                                     .is_free = true,
                                     .pid = 0};
  arr->curr_cap = allocated + 1;
}

void mba_push(mblock_array *arr, unsigned int size, unsigned int pid) {
  mem_block *blocks = arr->data;
  int data_location = -1;
  for (int i = 0; i < arr->curr_cap; i++) {
    if (blocks[i].is_free && blocks[i].capacity >= size) {
      data_location = i;
      break;
    }
  }

  if (data_location != -1) {
    blocks[data_location].is_free = false;
    blocks[data_location].pid = pid;
    if (blocks[data_location].capacity != size) {
      if (arr->curr_cap + 1 == arr->max_cap) {
        mba_realloc(arr);
        blocks = arr->data;
      }
      mem_block new_block = {.start = blocks[data_location].start + size,
                             .capacity = blocks[data_location].capacity - size,
                             .is_free = true,
                             .pid = 0};
      blocks[data_location].capacity = size;

      for (unsigned int i = arr->curr_cap; i > data_location + 1; i--) {
        blocks[i] = blocks[i - 1];
      }
      blocks[data_location + 1] = new_block;
      arr->curr_cap++;
    }
  } else {
    unsigned int total_free = 0;
    for (unsigned int i = 0; i < arr->curr_cap; i++) {
      if (arr->data[i].is_free)
        total_free += arr->data[i].capacity;
    }
    if (total_free >= size) {
      mba_compact(arr);
      mba_push(arr, size, pid);
    } else {
      printf("Not enough memory.\n");
    }
  }
}

void mba_dealloc(mblock_array *arr, unsigned int pid) {
  for (unsigned int i = 0; i < arr->curr_cap; i++) {
    if (arr->data[i].pid == pid) {
      arr->data[i].pid = 0;
      arr->data[i].is_free = true;

      if (i + 1 < arr->curr_cap && arr->data[i + 1].is_free) {
        arr->data[i].capacity += arr->data[i + 1].capacity;
        for (unsigned int j = i + 1; j < arr->curr_cap - 1; j++) {
          arr->data[j] = arr->data[j + 1];
        }
        arr->curr_cap--;
      }

      if (i > 0 && arr->data[i - 1].is_free) {
        arr->data[i - 1].capacity += arr->data[i].capacity;
        for (unsigned int j = i; j < arr->curr_cap - 1; j++) {
          arr->data[j] = arr->data[j + 1];
        }
        arr->curr_cap--;
        i--;
      }
    }
  }
}

void mba_print(mblock_array *arr) {
  printf("\n%-10s %-10s %-10s\n", "Address", "Size", "Status");
  printf("------------------------------\n");
  for (unsigned int i = 0; i < arr->curr_cap; i++) {
    mem_block b = arr->data[i];
    if (b.is_free) {
      printf("%-10u %-10u Free\n", b.start, b.capacity);
    } else {
      printf("%-10u %-10u Process %u\n", b.start, b.capacity, b.pid);
    }
  }
  printf("\n");
}

void mba_free(mblock_array *arr) { free(arr->data); }

void mba_insert(mblock_array *arr, unsigned int index, mem_block block) {
  if (arr->curr_cap == arr->max_cap) {
    mba_realloc(arr);
  }
  for (unsigned int i = arr->curr_cap; i > index; i--) {
    arr->data[i] = arr->data[i - 1];
  }
  arr->data[index] = block;
  arr->curr_cap++;
}

int main(void) {
  unsigned int total_mem;
  printf("Enter total memory size: ");
  scanf("%u", &total_mem);

  mblock_array arr;
  mba_init(&arr, 8, total_mem);

  arr.data[0] = (mem_block){
      .start = 0, .capacity = total_mem, .is_free = false, .pid = 0};
  arr.curr_cap = 1;

  unsigned int num_holes;
  printf("Enter number of initial holes: ");
  scanf("%u", &num_holes);

  for (unsigned int i = 0; i < num_holes; i++) {
    unsigned int start, size;
    printf("Hole %u start: ", i + 1);
    scanf("%u", &start);
    printf("Hole %u size: ", i + 1);
    scanf("%u", &size);

    for (unsigned int j = 0; j < arr.curr_cap; j++) {
      mem_block *b = &arr.data[j];
      if (start >= b->start && start < b->start + b->capacity) {
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
        break;
      }
    }
  }

  mba_print(&arr);

  int choice;
  do {
    printf("\n--- Memory Manager ---\n");
    printf("1. Allocate\n");
    printf("2. Deallocate\n");
    printf("3. Display Memory Map\n");
    printf("4. Compact\n");
    printf("5. Exit\n");
    printf("Choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1: {
      unsigned int pid, size;
      printf("Process ID: ");
      scanf("%u", &pid);
      printf("Size: ");
      scanf("%u", &size);
      mba_push(&arr, size, pid);
      break;
    }
    case 2: {
      unsigned int pid;
      printf("Process ID: ");
      scanf("%u", &pid);
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
      break;
    default:
      printf("Invalid choice.\n");
    }
  } while (choice != 5);

  mba_free(&arr);
  return 0;
}
