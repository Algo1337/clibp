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

int json_Append(json_t j, str_t path, str_t key, element e) {
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

		if(key->data[0] == '"' && key->data[key->idx - 1] == '"') {
			str_TrimAt(key, 0);
			str_TrimAt(key, key->idx - 1);
		}

		if(value->data[0] == '"' && value->data[value->idx - 1] == '"') {
			str_TrimAt(value, 0);
			str_TrimAt(value, value->idx - 1);
		}

		if(((str_t)args->arr[1])->data[0] == '{') {
			str_Append(path, args->arr[0]);
			str_Append(current, args->arr[0]);

			subs++;
			if(!str_Trim(args->arr[1], '"'))
				printf("FAILED\n");

			str_Trim(args->arr[1], '{');
			str_Trim(args->arr[2], '"');

			json_Append(json, path, args->arr[1], new_str(strdup(((str_t)args->arr[2])->data), 0));
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

			json_Append(json, path, key, new_str(strdup(value->data), 0));
			str_Remove(path, path->idx - current->idx, path->idx);
			continue;
		}

		if(str_StartsWith(args->arr[1], "[")) {
			int start = str_FindChar((str_t)rjson, '[', arrs), end = str_FindChar((str_t)rjson, ']', arrs);
			char *sub = str_GetSub((str_t)rjson, start + 1, end);
			json_Append(json, path, (str_t)args->arr[0], new_str(sub, 0));
			arrs++;
			continue;
		}

		str_Trim(value, '"');
		json_Append(json, path, key, new_str(strdup(value->data), 0));

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
		if(!strcmp(((jfield_t)geo->fields->arr[i])->path->data, "/")) {
			printf("[%d] Path: %s | %s => %s\n", i, ((jfield_t)geo->fields->arr[i])->path->data, ((jfield_t)geo->fields->arr[i])->key->data, ((jfield_t)geo->fields->arr[i])->value->data);
			continue;
		}

		printf("[%d] Path: %s | %s => %s\n", i, ((jfield_t)geo->fields->arr[i])->path->data, ((jfield_t)geo->fields->arr[i])->key->data, ((jfield_t)geo->fields->arr[i])->value->data);
	}
}
