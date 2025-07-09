#pragma once

#include "chr.h"

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

str_T   *new_str(const str data, int stack);
str_T   *str_Set(str_T *s);
str     str_Get(str_T *s);
size_t  str_Len(str_T *s);
int		str_Append(str_T *s, str_T *n);
int     str_iAppend(str_T *s, int num);
int		str_Remove(str_T *s, int start, int end);
int     str_IsASCII(str_T *s);
int     str_Is(str_T *s, const str q);
int     str_Contains(str_T *s, const str q);
int     str_IsNumber(str_T *s);
int     str_Strip(str_T *s);
int     str_Insert(str_T *s, int pos, const character ch);
int     str_CountChar(str_T *s, const character ch);
int     str_FindChar(str_T *s, const character ch, int match_count);
int     str_Trim(str_T *s, const character ch);
int     str_TrimAt(str_T *s, int pos);
int     str_ReplaceChar(str_T *s, const character find, const character replace);
int     str_CountString(str_T *s, const str find);
int     str_FindString(str_T *s, const str find, int match_count);
int     str_StartsWith(str_T *s, const str q);
int     str_EndsWith(str_T *s, const str q);
int     str_IsLowercase(str_T *s);
int     str_IsUppercase(str_T *s);
int     str_ToLowercase(str_T *s);
int     str_ToUppercase(str_T *s);
str     str_GetSub(str_T *s, int start, int end);
void	str_Destruct(str_T *s);
