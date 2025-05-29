#define STRING_CLIBP
#define ARRAY_CLIBP
#define MAP_CLIBP
#define JSON_CLIBP
#include "clibp.h"

int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("[ - ] Error, Invalid arguments provided.....!\nUsage %s <json_txt_file>\n", argv[0]);
		return 1;
	}

    map_t n = new_map();
    str txt = ReadFileContent(argv[1]);
    if(!txt)
        return 1;

	json_t geo = DecodeJSON(new_str(txt, 0));
	for(int i = 0; i < geo->fields->idx; i++) {
		if(((jfield_t)geo->fields->arr[i])->type == J_ARRAY) {
			printf("[%d] Path: %s | %s: ", i, ((str_t)((jfield_t)geo->fields->arr[i])->path)->data, ((str_t)((jfield_t)geo->fields->arr[i])->key)->data);
			arr_t a = ((jfield_t)geo->fields->arr[i])->value;
			if(a->idx < 1) {
				printf("Error, None found\n");
				break;
			}

			for(int c = 0; c < a->idx; c++) {
				str_Trim(((str_t)a->arr[c]), '\\');
				str_Trim(((str_t)a->arr[c]), '"');
				printf("%s, ", ((str_t)a->arr[c])->data);
			}
			printf("\n");
		}

		if(!strcmp(((jfield_t)geo->fields->arr[i])->path->data, "/") && ((jfield_t)geo->fields->arr[i])->type == J_STRING) {
			printf("[%d] Path: %s | %s => %s\n", i, ((str_t)((jfield_t)geo->fields->arr[i])->path)->data, ((str_t)((jfield_t)geo->fields->arr[i])->key)->data, ((str_t)((jfield_t)geo->fields->arr[i])->value)->data);
			continue;
		}
	}

	json_Destruct(geo);
}

