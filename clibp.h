#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "str/str.h"
#include "arr/arr.h"
#include "map/map.h"

#if defined(STRING_CLIBP)
	typedef str_T *str_t;
#endif

#if defined(ARRAY_CLIBP)
	typedef arr_T *arr_t;

	arr_t str_SplitAt(str_T *s, const character ch);
#endif

#if defined(MAP_CLIBP)
	typedef map_T *map_t;
	typedef field_T *field_t;
#endif

#if defined(JSON_CLIBP)
	typedef map_T *json_t;
	typedef jfield_T *jfield_t;
#endif

char *ReadFileContent(const char *filepath);
