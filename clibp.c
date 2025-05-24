#define STRING_CLIBP
#define ARRAY_CLIBP
#define MAP_CLIBP
#define JSON_CLIBP
#include "clibp.h"

/* Temporary Function */
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

/* Construct String Using Array */
str_t arr_Join(arr_t a, str delim) {
	if(!a)
		return NULL;

	str_t buff = new_str(NULL, 0);
	for(int i = 0; i < a->idx; i++)
		str_Append(buff, a->arr[i]);

	return buff;
}

/* Split String Using Char */
arr_t str_SplitAt(str_t s, const character ch) {
    if(!s || !s->data)
        return NULL;

    arr_T *list = new_arr(NULL, 0);
    int pos = 0, last = 0, match = 0;
    while((pos = str_FindChar(s, ch, match)) != -1) {
        str sub = str_GetSub(s, !last ? last : last + 1, pos);
        arr_Append(list, new_str(sub, 0));
        last = pos;
        match++;
    }

	str sub = str_GetSub(s, last + 1, s->idx);
	arr_Append(list, new_str(sub, 0));

    if(list->idx > 0)
        return list;

    arr_Destruct(list, str_Destruct);
    return list;
}
