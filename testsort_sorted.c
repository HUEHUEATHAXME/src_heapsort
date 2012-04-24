#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include "sort.h"
#include "time.h"

typedef struct data data_t;
struct data {
    int key;
};

data_t *data;

void fatal_error(char *msg, ...)
{
    va_list ap;

    va_start(ap, msg);
    vprintf(msg, ap);
    
    exit(1);
}


void dump_data(int numelements)
{
    int i;

    for (i = 0; i < numelements; i++) {
        printf("%d: %d\n", i, data[i].key);
    }
}

static int cmpfunc(data_t *d, int a, int b)
{
    return d[a].key - d[b].key;
}

static int cpyfunc(data_t *from, int a, data_t *to, int b)
{
    memcpy(to+b, from+a, sizeof(data_t));
    return 0;
}


int main(int argc, char **argv)
{
    int i, j, maxnum;
    FILE *f;
    unsigned long long t1, t2;

    if (argc <= 2)
        fatal_error("Usage: %s <num elements to sort> <output file>\n", argv[0]);

    f = fopen(argv[2], "w");
    if (f == NULL)
        fatal_error("Unable to open output file '%s'\n", argv[2]);

    maxnum = atol(argv[1]);
    if (maxnum <= 0)
        fatal_error("Usage: %s <num elements to sort> <output file>\n", argv[0]);

    data = (data_t*) malloc(sizeof(data_t)*maxnum);
    if (data == NULL)
        fatal_error("Unable to allocate memory\n");

    srand(121212);
    for (i = 8; i <= maxnum; i *= 2) {
        for (j = 0; j < i; j++)
            data[j].key = rand();

        /* Sort first */
        sort(data, 
             0,                 /* Sort from this index */
             i-1,               /* up to and including this index */
             (cmpfunc_t) cmpfunc, /* Cast to avoid compiler warning */
             (cpyfunc_t) cpyfunc, /* Cast to avoid compiler warning */
             sizeof(data_t));
        
        /* and then measure cost of sorting a sorted array */
        t1 = gettime();
        sort(data, 
             0,                 /* Sort from this index */
             i-1,               /* up to and including this index */
             (cmpfunc_t) cmpfunc, /* Cast to avoid compiler warning */
             (cpyfunc_t) cpyfunc, /* Cast to avoid compiler warning */
             sizeof(data_t));
        t2 = gettime();
        fprintf(f, "%d %d\n", i, (int)(t2-t1));
    }
    fclose(f);
}
