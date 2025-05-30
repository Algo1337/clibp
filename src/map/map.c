#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "map.h"

map_T *new_map(void) {
    map_T *m = (map_T *)malloc(sizeof(map_T));

    m->fields = new_arr(NULL, 0);

    return m;
}

int map_Search(map_T *m, str key) {
	if(!m || !key)
		return -1;

	for(int i = 0; i < m->fields->idx; i++) {
		if(!strcmp(((field_T *)m->fields->arr[i])->key->data, key))
			return i;
	}

	return -1;
}

int map_Append(map_T *m, str key, str value) {
    if(!m || !key || !value)
        return -1;

    field_T *field = (field_T *)malloc(sizeof(field_T));

    field->key = new_str(key, 0);
    field->value = new_str(value, 0);

    if(!arr_Append(m->fields, (void *)field)) {
        printf("[ - ] Error, Unable to append to map....!\n");
        return 0;
    }

    return 1;
}

void field_Destruct(field_T *f) {
	if(!f)
		return;

	if(f->key) {
		free(f->key);
		f->key = NULL;
	}

	if(f->value) {
		free(f->value);
		f->value = NULL;
	}

	free(f);
}

void map_Destruct(map_T *m, void *destructor) { 
    if(!m)
        return;

    if(m->fields) {
        for(int i = 0; i < m->fields->idx; i++) {
            if(!m->fields->arr[i])
                break;

            str_Destruct(((field_T *)m->fields->arr[i])->key);

            if(destructor)
                ((void *(*)(void *))destructor)(((field_T *)m->fields->arr[i])->value);
            else
				free(((field_T *)m->fields->arr[i])->value);

            m->fields->arr[i] = NULL;
        }

        arr_Destruct(m->fields, NULL);
    }

    free(m);
}
