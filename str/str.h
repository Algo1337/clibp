#pragma once

#include "../arr/arr.h"

typedef char *str;
typedef char character;

typedef enum str_Utils {
    STR_SET,
    STR_GET,
    STR_FINDCHAR
} str_Utils;

typedef struct str_T {
    str     data;
    int     idx;
    int     lock;
    struct {
        int stack, heap;
    } location;
} str_T;

str_T *new_str(const str data, int stack);
int str_Insert(str_T *s, int pos, const character ch);
int str_CountChar(str_T *s, const character ch);
int str_FindChar(str_T *s, const character ch, int match_count);
int str_Trim(str_T *s, const character ch);
int str_TrimAt(str_T *s, int pos);
int str_ReplaceChar(str_T *s, const character find, const character replace);
int str_FindString(str_T *s, const str find, int match_count);
char *str_GetSub(str_T *s, int start, int end);
