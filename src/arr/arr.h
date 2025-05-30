#pragma once

#include "../str/str.h"

typedef void *element;
typedef char *str;
typedef void **arr;

typedef struct arr_T {
    arr     arr;
    int     idx;
    int     lock;
    struct {
        int stack, heap;
    } location;
} arr_T;

/*
	@[DOC]
		Create a new instanse of arr_T *
*/
arr_T *new_arr(const arr arr, int stack);

/*
	@[DOC]
		Check if the array contains a element
*/
int arr_Contains(arr_T *a, element q);

/*
	@[DOC]
		Append a new element to array
*/
int arr_Append(arr_T *a, element q);

int arr_Remove(arr_T *a, int pos, void *destructor);

/*
	@[DOC]
		Merge a new array into the main array
*/
int arr_Merge(arr_T *a, arr r);

/*
	@[DOC]
		Destruct arr_T struct
*/
void arr_Destruct(arr_T *a, void *destructor);
