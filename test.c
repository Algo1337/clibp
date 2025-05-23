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

	arr_t path 		= new_arr(NULL, 0);
	int arrs 		= 0;
	int subs 		= 0;

	arr_Append(path, new_str(strdup("/"), 0));
	for(int i = 0; i < lines->idx; i++) {
		if(!lines->arr[i])
			break;

		str_t rfield = lines->arr[i];
		arr_t args = str_SplitAt(rfield, ':');

		if(args->idx != 2)
			continue;

		str_Trim(args->arr[0], '"');
		if(str_StartsWith(args->arr[1], "{")) {
			arr_Append(path, args->arr[0]);

			int start = str_FindChar((str_t)args->arr[1], '{', 0);
			str_TrimAt(args->arr[1], start);

			subs++;
			str_Trim(args->arr[1], '"');
			str_Trim(args->arr[2], '"');
			json_Append(json, (str_t)path->arr[path->idx - 1], (str_t)args->arr[1], new_str(strdup(((str_t)args->arr[2])->data), 0));
		}

		if(str_EndsWith(args->arr[1], "}")) {
			arr_Remove(path, path->idx - 1, str_Destruct);
			str_Trim((str_t)args->arr[1], '}');
			if(i == lines->idx - 1)
				break;
		}

		if(str_StartsWith(args->arr[1], "[")) {
			int start = str_FindChar((str_t)rjson, '[', arrs), end = str_FindChar((str_t)rjson, ']', arrs);
			char *sub = str_GetSub((str_t)rjson, start + 1, end);
			json_Append(json, (str_t)path->arr[path->idx - 1], (str_t)args->arr[0], new_str(sub, 0));
			arrs++;
			continue;
		}

		str_Trim(args->arr[1], '"');
		json_Append(json, (str_t)path->arr[path->idx - 1], (str_t)args->arr[0], new_str(strdup(((str_t)args->arr[1])->data), 0));

		arr_Destruct(args, str_Destruct);
	}

	arr_Destruct(lines, str_Destruct);
	arr_Destruct(path, str_Destruct);
	return json;
}

int main() {
    map_t n = new_map();
    str txt = ReadFileContent("test.txt");
    if(!txt)
        return 1;

	json_t geo = DecodeJSON(new_str(txt, 0));
	for(int i = 0; i < geo->fields->idx; i++) {
		printf("%s => %s\n", ((jfield_t)geo->fields->arr[i])->key->data, ((jfield_t)geo->fields->arr[i])->value->data);
	}
}
