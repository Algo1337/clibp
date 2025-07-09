#define CLIBP
#include "../clibp.h"

json_t new_json(void) {
	json_t json = new_map();

	return json;
}

int json_Contains(json_t j, str_t path, str_t key) {
	if(!j || !key)
		return -1;

	for(int i = 0; i < j->fields->idx; i++) {
		jfield_t field = j->fields->arr[i];

		if(path != NULL) {
			if(!strcmp(field->path->data, path->data) && !strcmp(field->key->data, key->data))
				return i;
			
			continue;
		}
		
		if(!strcmp(field->key->data, key->data))
			return i;
	}

	return -1;
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

		if(key->data[0] == '"' && key->data[key->idx - 1] == '"') {
			str_Trim(key, '"');
		}

		if((value->data[0] == '"')) {
			ctype = J_STRING;
			str_TrimAt(value, 0);
		} else if(value->data[1] == '"') {
			str_TrimAt(value, 1);
		}
		
		if(value->data[value->idx - 1] == '"') {
			str_TrimAt(value, value->idx - 1);
		} else if(value->data[value->idx - 2] == '"') {
			str_TrimAt(value, value->idx - 2);
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

void jfield_Destruct(jfield_t f) {
	if(!f)
		return;

	if(f->path)
		free(f->path);

	if(f->key)
		free(f->key);

	if(f->value)
		free(f->value);

	free(f);
}

void json_Destruct(json_t m) {
	if(!m)
		return;

	if(!m->fields->arr) {
		for(int i = 0; i < m->fields->idx; i++) {
			jfield_t field = m->fields->arr[i];

			switch((int)field->type) {
				case J_STRING:
					str_Destruct(field->key);
					str_Destruct(field->value);
					break;
				case J_INT:
					free(field->value);
					break;
				case J_ARRAY:
					arr_Destruct(field->value, str_Destruct);
					break;
				defualt:
					printf("[ - ] Error, Unable to delete: %s from map\n", field->key->data);
			}
		}
	}

	free(m);
}