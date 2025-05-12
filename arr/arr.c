#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "arr.h"

arr_T *new_arr(const arr arr, int stack) {
    arr_T *a = (arr_T *)malloc(sizeof(arr_T));
    memset(a, '\0', sizeof(arr_T));

    if(!arr)
        a->arr = malloc(sizeof(element) * 1);
    else
        a->arr = arr;

    if(stack)
        a->location.stack = 1;
    else 
        a->location.heap = 1;

	if(arr)
	    for(a->idx = 0; arr[a->idx] != NULL; a->idx++);

    return a;
}

int arr_Contains(arr_T *a, const str q) {
    if(!a || !q)
        return -1;

    for(int i = 0; i < a->idx; i++) {
        if(!strcmp((char *)a->arr[i], q))
            return i;
    }

    return -1;
}

int arr_Append(arr_T *a, const str q) {
    if(!a || !q)
        return -1;

    a->arr[a->idx] = q;
    a->idx++;
    a->arr = (arr)realloc(a->arr, sizeof(element) * (a->idx + 1));

    return 1;
}
