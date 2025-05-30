#define CLIBP
#include "../src/clibp.h"

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
	int pos;

	for(int i = 0; i < geo->fields->idx; i++) {
		printf("%s: %s\n", ((str_t)((jfield_t)geo->fields->arr[i])->key)->data, ((str_t)((jfield_t)geo->fields->arr[i])->value)->data);
	}

	json_Destruct(geo);
}

