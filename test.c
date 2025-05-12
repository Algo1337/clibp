#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "str/str.h"

char *ReadFileContent(const char *filepath) {
    if(!filepath)
        return NULL;

    FILE *file = fopen(filepath, "r");
    if(!file)
        return NULL;

    fseek(file, 0L, SEEK_END);
    size_t sz = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char *data = (char *)malloc(sz);
    if(!data)
        return NULL;
    memset(data, '\0', sz);

    fread(data, 1, sz, file);
    if(strlen(data) > 0)
        return data;

    free(data);
    return NULL;
}

int main() {
    str txt = ReadFileContent("test.txt");
    if(!txt)
        return 1;

    str_T *t = new_str(txt, 0);
	arr_T *arr = new_arr(NULL, 0);
    str_Trim(t, '{');
    str_Trim(t, '}');
    str_Trim(t, '"');
    str_Trim(t, '\\');

    str_ReplaceChar(t, '/', '-');
    for(int pos = 0, count = 0; (pos = str_FindChar(t, ':', count)) != -1; count++) {
        if(str_Insert(t, pos + 1, ' ') == -1)
            printf("[ x ] Failed to insert character\n");
    }

    for(int pos = 0, count = 0; (pos = str_FindChar(t, ',', count)) != -1; count++) {
        if(str_Insert(t, pos + 1, ' ') == -1)
            printf("[ x ] Failed to insert character\n");

		int end = str_FindChar(t, ',', count + 1);
		char *sub = str_GetSub(t, pos + 2, end);
		arr_Append(arr, sub);
    }

	for(int i = 0; i < arr->idx; i++) {
		printf("%s\n", (str)arr->arr[i]);
	}

    printf("\nStack: %s | %s\n", t->location.stack ? "True":"False", t->data);
    return 0;
}
