#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "str.h"

str_T *new_str(const str data, int stack) {
    str_T *s = (str_T *)malloc(sizeof(str_T));

    s->data = (char *)data;

    if(stack)
        s->location.stack = 1;
    else
        s->location.heap = 1;

    s->idx = (!data ? 0 : strlen(data));

    return s;
}

str_T *str_Set(str_T *s) {
    if(!s)
        return NULL;

    if(s->data) {
        memset(s->data, '\0', s->idx);
        free(s->data);
        s->data = NULL;
    }

    s->data = (char *)malloc(1);
    s->idx = 1;

    return s;
}

str str_Get(str_T *s) {
    return (str)s->data;
}

size_t str_Len(str_T *s) {
    return s->idx;
}

int str_IsASCII(str_T *s) {
    if(!s || !s->data)
        return 0;

    for(int i = 0; i < s->idx; i++) {
        if(s->data[i] <= 0 || s->data[i] >= 127)
            return 0;
    }

    return 1;
}

int str_Is(str_T *s, const str q) {
    if(!s || !s->data)
        return 0;

    return (!strcmp(s->data, q));
}

int str_Contains(str_T *s, const str q) {
    if(!s || !s->data)
        return 0;

    return (strstr(s->data, q) != 0 ? 1 : 0);
}

int str_IsNumber(str_T *s) {
    if(!s || !s->data)
        return 0;

    for(int i = 0; s->data[i] != '\0'; i++) {
        if(s->data[i] < '0' || s->data[i] > '9')
            return 0;
    }

    return 1;
}

int str_Strip(str_T *s) {
    if(!s || !s->data)
        return -1;

    int start = 0, end = s->idx - 1;
    while(start <= (s->idx - 1) && s->data[start] == ' ')
        start++;

    while(end >= start && s->data[end] == ' ')
        end--;

    int new_len = end - start + 1;
    char *data = (char *)malloc(new_len);
    memset(data, '\0', new_len);

    for(int i = 0; i < new_len; i++)
        data[i] = s->data[start + i];

    free(s->data);
    s->data = data;
    s->idx = new_len;

    return new_len;
}

int str_Insert(str_T *s, int pos, const character ch) {
    if(!s || ch == '\0' || s->location.stack)
        return -1;

    char *data = (char *)malloc(s->idx + 1);
    int idx = 0;
    for(int i = 0; i < s->idx; i++) {
        if(i == pos) {
            data[idx] = ch;
            idx++;
            data = (char *)realloc(data, idx + 1);
        }

        data[idx] = s->data[i];
        idx++;
        data = (char *)realloc(data, idx + 1);
    }

    data[idx] = '\0';

    if(s->data)
        free(s->data);

    s->data = data;
    s->idx = idx;
    return 1;
}

int str_CountChar(str_T *s, const character ch) {
    if(!s)
        return -1;

    int count = 0;
    for(int i = 0; i < s->idx; i++) {
        if(s->data[i] == '\0')
            break;

        if(s->data[i] == ch)
            count++;
    }

    return count;
}

int str_FindChar(str_T *s, const character ch, int match_count) {
    if(!s)
        return -1;

    int count = 0;
    for(int i = 0; i < s->idx; i++) {
        if(s->data[i] == '\0')
            break;
        
        if(s->data[i] == ch) {
            if(match_count == count) {
                return i;
            }

            count++;
        }
    }

    return -1;
}

int str_Trim(str_T *s, const character ch) {
    if(!s || s->idx < 2 || s->location.stack)
        return -1;

    char *data = (char *)malloc(1);
    int idx = 0;

    for(int i = 0; i < s->idx; i++) {
        if(s->data[i] == '\0')
            break;

        if(s->data[i] == ch)
            continue;

        data[idx] = s->data[i];
        idx++;
        data = (char *)realloc(data, idx + 1);
    }

    data[idx] = '\0';

    if(s->data)
        free(s->data);

    s->data = data;
    s->idx = idx;

    return idx;
}

int str_TrimAt(str_T *s, int pos) {
    if(!s || !s->data || s->idx < 2 || s->location.stack)
        return -1;

    char *data = (char *)malloc(1);
    int idx = 0;

    for(int i = 0; i < s->idx; i++) {
        if(s->data[i] == '\0')
            break;

        if(i == pos)
            continue;

        data[idx] = s->data[i];
        idx++;
        data = (char *)realloc(data, idx + 1);
    }

    data[idx] = '\0';

    if(s->data)
        free(s->data);

    s->data = data;
    s->idx = idx;

    return idx;
}

int str_ReplaceChar(str_T *s, const character find, const character replace) {
    if(!s || !s->data || s->location.stack)
        return 0;

    char *data = (char *)malloc(1);
    memset(data, '\0', 1);
    int idx = 0;

    for(int i = 0; i < s->idx; i++) {
        if(s->data[i] == find) {
            data[idx] = replace;
            idx++;
            data = (char *)realloc(data, idx + 1);
            continue;
        }

        data[idx] = s->data[i];
        idx++;
        data = (char *)realloc(data, idx + 1);
    }

    data[idx] = '\0';
    if(s->data)
        free(s->data);

    s->data = data;
    s->idx = idx;

    return 1;
}

int str_FindString(str_T *s, const str find, int match_count) {
    if(!s || !s->data || s->idx < 2)
        return -1;

    int len = strlen(find);
    if(len > s->idx)
        return -1;
        
    int count = 0;
    for(int i = 0; i < s->idx; i++)
    {
        if(i + len - 1 >= s->idx)
            break;

        if(s->data[i] == find[0] && s->data[i + len - 1] == find[len - 1]) {
            for(int c = 0; c < len; c++) {
                if(count == match_count)
                    return i;

                if(s->data[i + c] == find[c] && c == len -1) {
                    count++;
                }
            }
        }
    }

    return -1;
}

int str_StartsWith(str_T *s, const str q) {
    if(!s || !s->data)
        return 0;

    int len = strlen(q);
    if(s->data[0] == q[0] && s->data[len - 1] == q[len - 1])
        for(int i = 0; i < len; i++)
            if(s->data[i] == q[i] && i == len - 1)
                return 1;

    return 0;
}

int str_EndsWith(str_T *s, const str q) {
    if(!s || !s->data)
        return 0;

    int len = strlen(q);
    if(s->data[s->idx - len - 1] == q[0] && s->data[s->idx - 1] == q[len - 1])
        for(int i = 0; i < len; i++)
            if(s->data[s->idx - i] == q[i] && i == len)
                return 1;

    return 0;
}

int str_IsLowercase(str_T *s) {
    if(!s || !s->data)
        return 0;

    for(int i = 0; i < s->idx; i++) {
        if(s->data[i] == '\0')
            break;

        if(s->data[i] < 'a' || s->data[i] > 'z')
            return 0;
    }

    return 1;
}

int str_IsUppercase(str_T *s) {
    if(!s || !s->data)
        return 0;

    for(int i = 0; i < s->idx; i++) {
        if(s->data[i] == '\0')
            break;

        if(s->data[i] < 'A' || s->data[i] > 'Z')
            return 0;
    }

    return 1;
}

str str_GetSub(str_T *s, int start, int end) {
    if(!s || !s->data || s->idx < 2)
        return NULL;

    if(end > s->idx)
        return NULL;

    char *data = (char *)malloc(1);
    int idx = 0;

    for(int i = start; i < end; i++) {
        data[idx] = s->data[i];
        idx++;
        data = (char *)realloc(data, idx + 1);
    }

    data[idx] = '\0';
    return data;
}

int str_ReplaceString(str_T *s, const str find, const str replace) {

}

int str_Join(str_T *s, arr r) {

}

void str_Destruct(str_T *s) {
	if(!s)
		return;

	if(s->data) {
		free(s->data);
		s->data = NULL;
	}

	free(s);
}
