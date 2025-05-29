#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "str/str.h"
#include "arr/arr.h"
#include "map/map.h"

#define ____Inline____(return_type, function_body) \
    ({ return_type __fn__ function_body __fn__; })

#if defined(STRING_CLIBP)
	typedef str_T *str_t;
#endif

#if defined(ARRAY_CLIBP)
	typedef arr_T *arr_t;

	arr_t str_SplitAt(str_t s, const character ch);
#endif

#if defined(MAP_CLIBP)
	typedef map_T *map_t;
	typedef field_T *field_t;
#endif

#if __has_include("map/map.h") && defined(JSON_CLIBP)
	typedef struct jfield_T {
		str_T       *path;
		str_T       *key;
		void        *value;
		JSON_TYPE	type;
	} jfield_T;

	typedef JSON_TYPE json_types;
	typedef map_T *json_t;
	typedef jfield_T *jfield_t;
	
	json_t new_json(void);
	int json_Append(json_t j, str_t path, str_t key, element e, json_types type);
	json_t DecodeJSON(const str_T *rjson);
	void jfield_Destruct(jfield_t f);
	void json_Destruct(json_t m);
#else
	#error "Missing Header Files"
#endif

char *ReadFileContent(const char *filepath);
