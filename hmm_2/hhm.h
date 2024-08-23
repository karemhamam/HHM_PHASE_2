#ifndef HMM_H                // include guard.
#define HMM_H

#include <stddef.h>          // to use size_t

void *HmmAlloc(size_t size); // declaration of HmmAlloc func.
void HmmFree(void *ptr);     // declaration of HmmFree func.
void *HmmRealloc(void *ptr , size_t size); // declaration of HmmRealloc func.
void *HmmCalloc(size_t num , size_t size); // declaration of HmmCalloc func.


#endif                       // end include guard HMM_H
