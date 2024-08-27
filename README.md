# Heap Memory Manager (HMM)

## Introduction

The Heap Memory Manager (HMM) provides a custom implementation of dynamic memory allocation that simulates a heap in user space without relying on kernel support. This educational implementation demonstrates the fundamentals of memory management and allocation techniques.

## Features

- **Dynamic Memory Allocation:** Custom implementation of `void *HmmAlloc(size_t size)` for allocating memory.
- **Memory Deallocation:** Custom implementation of `void HmmFree(void *ptr)` to free allocated memory.
- **Memory Management:** Simulates a heap using `sbrk()` to request memory from the kernel.
- **Memory Coalescing:** Merges adjacent free blocks to reduce fragmentation and improve memory utilization.
- **Efficient Allocation:** Minimizes system call overhead and memory fragmentation through careful heap management.

## Design

1. **Heap Management:**
   - Uses `sbrk()` to request a block of memory from the kernel.
   - Maintains pointers to the start (`heap_start`), end (`heap_end`), and current position (`current_brk`) of the heap.

2. **Block Header:**
   - Each allocated block includes a header (`block_header`) to manage size, allocation status, and linking to adjacent blocks.
   - The `block_header` structure is defined as:
     ```c
     typedef struct block_header {
         size_t size;
         int free;
         struct block_header *next;
     } block_header;
     ```

3. **Memory Allocation (HmmAlloc):**
   - Checks for zero-size requests and returns `NULL` if requested size is zero.
   - Searches for a free block that fits the requested size using a first-fit strategy.
   - Expands the heap if necessary and allocates a new block.
   - Updates the `current_brk` pointer and returns a pointer to the usable memory area.

4. **Memory Deallocation (HmmFree):**
   - Checks if the pointer is `NULL` and returns immediately if so.
   - Marks the block as free and attempts to merge adjacent free blocks to reduce fragmentation.

5. **Memory Reallocation (HmmRealloc):**
   - Allocates new memory if the pointer is `NULL`.
   - Copies data from the old block to the new block if the new size is larger.
   - Frees the old block once the data is copied.

6. **Memory Allocation with Initialization (HmmCalloc):**
   - Allocates memory and initializes it to zero.

## Flowcharts

#### Flowchart for `HmmAlloc`

![HmmAlloc](https://github.com/user-attachments/assets/4a6c7ce8-f0cd-4530-9ecf-5875c2c6e321)

#### Flowchart for `HmmFree`

![HmmFree](https://github.com/user-attachments/assets/be41acfc-c2a8-436f-ba39-e3fcd1defff0)

#### Flowchart for `HmmRealloc`

![HmmRealloc flow chart](https://github.com/user-attachments/assets/381c26e0-b986-42f1-8955-e3f676c616cc)

#### Flowchart for `HmmCalloc`

![HmmCalloc flow chart](https://github.com/user-attachments/assets/dd00e87a-f135-4a09-aaac-0a13dcbd7d4f)

### video showing the design and testing

drive link (https://drive.google.com/file/d/1UYj0FH9O_oj7dwnWS8wwVHwF0tNU_3l2/view?usp=sharing)

## Installation

1. Clone the repository and compile the code: git clone (https://github.com/karemhamam/HHM_PHASE_2/tree/master)

## Contributing

Contributions are welcome! 
If you have suggestions for new features or improvements, feel free to open an issue or submit a pull request.

## License

mahamedhamam15@gmail.com
