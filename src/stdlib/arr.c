#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../headers/arr.h"

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

int arr_Contains(arr_T *a, element q) {
    if(!a || !q)
        return -1;

    for(int i = 0; i < a->idx; i++) {
        if(!strcmp((char *)a->arr[i], q))
            return i;
    }

    return -1;
}

int arr_Append(arr_T *a, element q) {
    if(!a || !q)
        return 0;

    if(a->lock)
        return 0;

    a->arr[a->idx] = q;
    a->idx++;
    a->arr = (arr)realloc(a->arr, sizeof(element) * (a->idx + 1));
    a->arr[a->idx] = NULL;

    return 1;
}

int arr_Insert(arr_T *a, int pos, element q) {
    if(!a || !q)
        return -1;

    a->idx++;
    void **arr = (void **)malloc(sizeof(element) * a->idx);
    int idx = 0;

    for(int i = 0; i < a->idx; i++) {
        if(i == pos) {
            arr[idx] = q;
            continue;
        }

        arr[idx] = a->arr[i];
    }

    free(a->arr);
    a->arr = arr;
    
    return 1;
}

int arr_Remove(arr_T *a, int pos, void *destructor) {
	if(!a || pos < 0)
		return 0;

	void **arr = (void **)malloc(sizeof(void *) * 1);
	int idx = 0;

	for(int i = 0; i < a->idx; i++) {
		if(!a->arr[i])
			break;

		if(i == pos) {
			((void (*)(void *))destructor)(a->arr[i]);
			continue;
		}

		arr[idx] = a->arr[i];
		idx++;
		arr = (void **)realloc(arr, sizeof(void *) * (idx + 1));
	}

	free(a->arr);
	a->arr = arr;
	a->idx = idx;
	return 1;
}

int arr_Merge(arr_T *a, arr r) {
	if(!a || !r)
		return -1;

	int added = 0;
	for(int i = 0; r[i] != NULL; i++) {
		if(arr_Append(a, r[i]))
			added++;
	}

	return added;
}

int arr_Reverse(arr_T *a) {
    if(!a)
        return 0;

    void **arr = (void **)malloc(sizeof(element) * a->idx);
    int idx = 0;

    for(int i = a->idx; a->idx > i; i++)
        arr[idx] = a->arr[i];

    free(a->arr);

    a->arr = arr;
    return 1;
}

void arr_Destruct(arr_T *a, void *destructor) {
    if(!a)
        return;

    if(a->arr) {
        for(int i = 0; i < a->idx; i++) {
            if(!a->arr[i])
                break;

			if(destructor)
				((void *(*)(void *))destructor)(a->arr[i]);
			else
            	free(a->arr[i]);

            a->arr[i] = NULL;
        }

        free(a->arr);
    }

    free(a);
}
