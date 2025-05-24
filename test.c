#define STRING_CLIBP
#define ARRAY_CLIBP
#define MAP_CLIBP
#define JSON_CLIBP
#include "clibp.h"

typedef map_t json_t;
json_t new_json(void) {
	json_t json = new_map();

	return json;
}

int json_Append(json_t j, str_t path, str_t key, element e, json_types type) {
	if(!j)
		return 0;

	jfield_t field = (jfield_t)malloc(sizeof(jfield_t));
	if(!field)
		return 0;

	if(!path) {
		printf("NO PATH\n");
		return 0;
	}

	field->path = new_str(strdup(path->data), 0);
	field->key = new_str(strdup(key->data), 0);
	field->value = e;
	field->type = type;

	arr_Append(j->fields, field);
	return 1;
}


json_t DecodeJSON(const str_T *rjson) {
	if(!rjson ||!rjson->data)
		return NULL;

	str_TrimAt((str_t)rjson, 0);
	str_TrimAt((str_t)rjson, rjson->idx - 1);

	json_t json 	= new_json();
	arr_t lines 	= str_SplitAt((str_t)rjson, ',');

	str_t path 		= new_str(strdup("/"), 0);
	str_t current 	= new_str(NULL, 0);
	int arrs 		= 0;
	int subs 		= 0;

	for(int i = 0; i < lines->idx; i++) {
		if(!lines->arr[i])
			break;

		str_t rfield = lines->arr[i];
		arr_t args = str_SplitAt(rfield, ':');

		if(args->idx < 2)
			continue;

		str_t key = args->arr[0];
		str_t value = args->arr[1];
		json_types ctype;

		if(str_Contains(key, "\"")) {
			str_Trim(key, '"');
		}

		if(value->data[0] == '"' && value->data[value->idx - 1] == '"') {
			ctype = J_STRING;
			str_TrimAt(value, 0);
			str_TrimAt(value, value->idx - 1);
		}

		if(str_StartsWith(args->arr[1], "[")) {
			ctype = J_ARRAY;
			int start = str_FindChar((str_t)rjson, '[', arrs), end = str_FindChar((str_t)rjson, ']', arrs);
			str_t sub = new_str(strdup(str_GetSub((str_t)rjson, start + 1, end)), 0);
			arr_t arr;
			if(str_CountChar(sub, ',') > 0) {
				arr = str_SplitAt(sub, ',');
				str_Destruct(sub);
			} else {
				arr = new_arr(NULL, 0);
				arr_Append(arr, sub);
			}

			json_Append(json, path, (str_t)args->arr[0], arr, ctype);

			arrs++;
			i += arr->idx - 1;
			continue;
		}

		if(((str_t)args->arr[1])->data[0] == '{') {
			str_Append(path, args->arr[0]);
			str_Append(current, args->arr[0]);

			subs++;
			str_Trim(args->arr[1], '"');
			str_Trim(args->arr[1], '{');
			str_Trim(args->arr[2], '"');

			json_Append(json, path, args->arr[1], new_str(strdup(((str_t)args->arr[2])->data), 0), ctype);
			continue;
		}

		if(value->data[value->idx] == '}' || strstr(value->data, "}")) {
			if(str_Contains(value, "\"")) {
				int max_quotes = str_CountChar(value, '"');
				int start = str_FindChar(value, '"', 0), end = str_FindChar(value, '"', max_quotes - 1);
				str_TrimAt(value, start);
				str_TrimAt(value, end - 1);
			}

			str_TrimAt(value, value->idx - 1);

			json_Append(json, path, key, new_str(strdup(value->data), 0), ctype);
			str_Remove(path, path->idx - current->idx, path->idx);
			continue;
		}

		str_Trim(value, '"');
		json_Append(json, path, key, new_str(strdup(value->data), 0), ctype);

		arr_Destruct(args, str_Destruct);
	}

	arr_Destruct(lines, str_Destruct);
	str_Destruct(path);
	return json;
}

int main() {
    map_t n = new_map();
    str txt = ReadFileContent("test.txt");
    if(!txt)
        return 1;

	json_t geo = DecodeJSON(new_str(txt, 0));
	for(int i = 0; i < geo->fields->idx; i++) {
		if(((jfield_t)geo->fields->arr[i])->type == J_ARRAY) {
			printf("[%d] Path: %s | %s\n", i, ((str_t)((jfield_t)geo->fields->arr[i])->path)->data, ((str_t)((jfield_t)geo->fields->arr[i])->key)->data);
			arr_t a = ((jfield_t)geo->fields->arr[i])->value;
			if(a->idx < 1) {
				printf("Error, None found\n");
				break;
			}

			for(int c = 0; c < a->idx; c++) {
				printf("%s\n", ((str_t)a->arr[c])->data);
			}
		}

		if(!strcmp(((jfield_t)geo->fields->arr[i])->path->data, "/") && ((jfield_t)geo->fields->arr[i])->type == J_STRING) {
			printf("[%d] Path: %s | %s => %s\n", i, ((str_t)((jfield_t)geo->fields->arr[i])->path)->data, ((str_t)((jfield_t)geo->fields->arr[i])->key)->data, ((str_t)((jfield_t)geo->fields->arr[i])->value)->data);
			continue;
		}
	}
}

