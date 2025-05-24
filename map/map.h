#pragma once

#include "../str/str.h"
#include "../arr/arr.h"

typedef enum JSON_TYPE {
	J_STRING = 0x03543,
	J_INT = 0x03544,
	J_ARRAY = 0x03545
} JSON_TYPE;

typedef struct jfield_T {
    str_T       *path;
    str_T       *key;
    void        *value;
	JSON_TYPE	type;
} jfield_T;

typedef struct field_T {
    str_T       *key;
    str_T       *value;
} field_T;

typedef arr_T *fields_t;

typedef struct map_T {
    fields_t   fields;
} map_T;

map_T *new_map(void);
int map_Append(map_T *m, str key, str value);
void field_Destruct(field_T *f);
void map_Destruct(map_T *m, void *destructor);

