#include <stdio.h>
#include <stdlib.h>
#include "sort.h"

#define DEBUG_HEAPSORT             0

typedef struct heapsort heapsort_t;
struct heapsort {
    void *data;
    cmpfunc_t cmpfunc;
    cpyfunc_t cpyfunc;
    int datasize;
    int heapsize;
    int left;
    void *tmp;
};

#define heapindex(idx) ((idx) + h->left)


static void print_heap(heapsort_t *h, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        printf("%d:%d\n", i+1, ((int*)h->data)[i]);
    }
}

static void heap_check(heapsort_t *h)
{
    int index, child;

    for (index = 1; index <= h->heapsize/2; index++) {
        child = index*2;
        if (child <= h->heapsize &&
            h->cmpfunc(h->data, heapindex(child), heapindex(index)) > 0) {
            printf("child %d > parent %d\n", child, index);
        }
        if (child+1 <= h->heapsize && 
            h->cmpfunc(h->data, heapindex(child+1), heapindex(index)) > 0) {
            printf("child %d > parent %d\n", child+1, index);
        }
    }
}


static void percdown(heapsort_t *h, int index)
{
    int child;
    
    while ((index*2) <= h->heapsize ) {
        /* Find largest child */
        child = index*2;
        if ((child + 1) <= h->heapsize &&
            h->cmpfunc(h->data, heapindex(child+1), heapindex(child)) > 0)
            child++;

        /* If parent >= child we're done */
        if (h->cmpfunc(h->data, heapindex(index), heapindex(child)) >= 0) {
            break;
        }

        /* Swap parent and child */
        h->cpyfunc(h->data, heapindex(index), h->tmp, 0);
        h->cpyfunc(h->data, heapindex(child), h->data, heapindex(index));
        h->cpyfunc(h->tmp, 0, h->data, heapindex(child));

        /* and continue from child */
        index = child;
    }
}


void _heapsort(heapsort_t *h)
{
    int index;
    
    /* Heapify data */
    for (index = h->heapsize/2; index >= 1; index--) {
        percdown(h, index);
    }

    heap_check(h);

    /* Swap last and first and reduce heap size with 1.
       When we're done we have sorted the input */
    while (h->heapsize > 0) {
        h->cpyfunc(h->data, heapindex(1), h->tmp, 0);
        h->cpyfunc(h->data, heapindex(h->heapsize), 
                   h->data, heapindex(1));
        h->cpyfunc(h->tmp, 0, h->data, heapindex(h->heapsize));
        h->heapsize--;
        percdown(h, 1);
    }
}


int sort(void *data, 
         int left, int right,
         cmpfunc_t cmpfunc,
         cpyfunc_t cpyfunc,
         int datasize)
{
    heapsort_t *h;


    h = (heapsort_t*)malloc(sizeof(heapsort_t));
    if (h == NULL)
        goto error;
    h->data = data;
    h->cmpfunc = cmpfunc;
    h->cpyfunc = cpyfunc;
    h->datasize = datasize;
    h->left = left-1;
    h->heapsize = (right-left) + 1;
    h->tmp = malloc(datasize);
    if (h->tmp == NULL)
        goto error;

    _heapsort(h);

    free(h->tmp);
    free(h);

    return 0;
 error:
    if (h != NULL) {
        if (h->tmp != NULL)
            free(h->tmp);
        free(h);
    }
    return -1;
}
