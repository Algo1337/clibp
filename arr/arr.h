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

arr_T *new_arr(const arr arr, int stack);
int arr_Contains(arr_T *a, const str q);
int arr_Append(arr_T *a, const str q);
