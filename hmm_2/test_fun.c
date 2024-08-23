#include <stdio.h>
#include <string.h>
#include "hhm.h"

void test_all_functions();

int main() 
{
    test_all_functions();
    return 0;
}

void test_all_functions() 
{
    // Test 1: Basic Allocation
    printf("Test 1: Basic Allocation\n");
    void *ptr = HmmAlloc(100);
    if (ptr == NULL) 
    {
        printf("Allocation failed\n");
    } else 
    {
        printf("Allocation successful\n");
    }

    // Test 2: Reallocation
    printf("Test 2: Reallocation\n");
    ptr = HmmRealloc(ptr, 200);
    if (ptr == NULL) 
    {
        printf("Reallocation failed\n");
    } else 
    {
        printf("Reallocation successful\n");
    }

    // Test 3: Calloc
    printf("Test 3: Calloc\n");
    void *calloc_ptr = HmmCalloc(10, sizeof(int));
    if (calloc_ptr == NULL) 
    {
        printf("Calloc failed\n");
    } else 
    {
        printf("Calloc successful\n");
        // Verify that memory is zero-initialized
        int *int_ptr = (int *)calloc_ptr;
        int all_zero = 1;
        for (size_t i = 0; i < 10; ++i) 
        {
            if (int_ptr[i] != 0) 
            {
                all_zero = 0;
                break;
            }
        }
        if (all_zero) 
        {
            printf("Memory zero-initialized successfully\n");
        } else 
        {
            printf("Memory not zero-initialized\n");
        }
    }

    // Test 4: Free and Double Free
    printf("Test 4: Free and Double Free\n");
    HmmFree(ptr);
    // Double free
    HmmFree(ptr);  // This should ideally be handled gracefully
    printf("Double free test complete\n");

    // Test 5: Memory Overrun
    printf("Test 5: Memory Overrun\n");
    ptr = HmmAlloc(100);
    if (ptr == NULL) 
    {
        printf("Allocation failed\n");
    } else 
    {
        // Write beyond allocated memory
        memset(ptr, 0, 200);  // This could lead to issues if not handled properly
        HmmFree(ptr);
        printf("Memory overrun test complete\n");
    }

    // Test 6: Large Allocation
    printf("Test 6: Large Allocation\n");
    ptr = HmmAlloc(1024 * 1024);  // 1 MB allocation
    if (ptr == NULL) 
    {
        printf("Large allocation failed\n");
    } else 
    {
        printf("Large allocation successful\n");
        HmmFree(ptr);
    }
}
