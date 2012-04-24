#ifndef SORT_H
#define SORT_H

/* Comparison function. Returns a value:
   > 0  , if a > b
   == 0 , if a == b
   < 0  , if a < b */
typedef int (*cmpfunc_t)(void *data, int a, int b);

/* Copy element function.  NOTE: from/to may point to auxiliary
   storage. The implementation has to take this into account. For
   example, if you are sorting a list, to/from may point into the list
   or into auxilliary storage depending on the actual sorting
   algorithm used. */
typedef int (*cpyfunc_t)(void *from, int a, void *to, int b);

/* Sort interface */
int sort(void *data, 
         int left, int right,
         cmpfunc_t cmpfunc,
         cpyfunc_t cpyfunc,
         int datasize);

#endif  /* SORT_H */
