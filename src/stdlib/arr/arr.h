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

/*
	@[DOC]
		Remove an element idx from array with a destructor for the element
*/
int arr_Remove(arr_T *a, int pos, void *destructor);

/*
	@[DOC]
		Merge a new array into the main array
*/
int arr_Merge(arr_T *a, arr r);

/*
	@[DOC]
		Insert an element into the array at a position
*/
int arr_Insert(arr_T *a, int pos, element q);

/*
	@[DOC]
		Reverse an array
*/
int arr_Reverse(arr_T *a);

/*
	@[DOC]
		Destruct arr_T struct
*/
void arr_Destruct(arr_T *a, void *destructor);
