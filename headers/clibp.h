#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "str.h"
#include "arr.h"
#include "map.h"

#include "dir.h"
#include "file.h"


#if defined(CLIBP)
	#define INT_CLIBP	
	#define STRING_CLIBP
	#define ARRAY_CLIBP
	#define MAP_CLIBP
	#define JSON_CLIBP
#endif

#define ____Inline____(return_type, function_body) \
    ({ return_type __fn__ function_body __fn__; })

#if defined(INT_CLIBP)
	typedef int int_T;
	typedef int_T *int_t;
#endif

#if __has_include("str.h") && defined(STRING_CLIBP)
	typedef str_T 		*str_t;
#else
	#warning "Missing str header file"
#endif

#if __has_include("arr.h") && defined(ARRAY_CLIBP)
	typedef arr_T 		*arr_t;

	str_t arr_Join(arr_t a, str delim);
	arr_t str_SplitAt(str_t s, const character ch);
#else
	#warning "Missing arr header file"
#endif

#if __has_include("map.h") && defined(MAP_CLIBP)
	typedef map_T 		*map_t;
	typedef field_T 	*field_t;
#else
	#warning "Missing map header file"
#endif

#if __has_include("map.h") && defined(JSON_CLIBP)
	typedef struct jfield_T {
		str_T       *path;
		str_T       *key;
		void        *value;
		JSON_TYPE	type;
	} jfield_T;

	typedef JSON_TYPE 	json_types;
	typedef map_T 		*json_t;
	typedef jfield_T 	*jfield_t;
	
	json_t 		new_json(void);
	int 		json_Contains(json_t j, str_t path, str_t key);
	int 		json_Append(json_t j, str_t path, str_t key, element e, json_types type);
	json_t 		DecodeJSON(const str_T *rjson);
	void 		jfield_Destruct(jfield_t f);
	void 		json_Destruct(json_t m);
#else
	#warning "Missing Header Files"
#endif

typedef void *void_t;

char *ReadFileContent(const char *filepath);

void free_many(void **args);