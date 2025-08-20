#define CLIBP
#define STRING_CLIBP
#define ARRAY_CLIBP
#include "../../headers/clibp.h"

file_t create_file(str_T *name, size_t sz) {
	Item *file = (Item *)malloc(sizeof(Item));

	file->name = name;
	file->size = sz;
	file->is_file = 1;

	return file;
}

void file_Destruct(file_t file) {
	if(!file)
		return;

	if(file->name)
		str_Destruct(file->name);

	free(file);
}

dir_t open_dir(str_T *name) {
	Dir *d = (Dir *)malloc(sizeof(Dir));

	d->name = name;
	d->items = new_arr(NULL, 0);

	__get_dir_info__(d);
	return d;
}

dir_t create_dir(str_T *name) {
	Dir *d = (Dir *)malloc(sizeof(Dir));

	d->name = name;
	d->items = NULL;

	return d;
}

static void __get_dir_info__(dir_t d) {
    struct dirent *de;
    DIR *dr = opendir(d->name->data);
    if(!dr)
        return;

    while((de = readdir(dr)) != NULL) {
        struct stat st;
        if(stat(de->d_name, &st) == -1)
            continue;

        if(S_ISDIR(st.st_mode)) {
            arr_Append(d->items, create_dir(new_str(strdup(de->d_name), 0)));
			continue;
        }

        arr_Append(d->items, create_file(new_str(strdup(de->d_name), 0), st.st_size));
    }
}

void dir_Destruct(dir_t d) {
	if(!d)
		return;

	if(d->name)
		str_Destruct(d->name);

	if(d->items) {
		for(int i = 0; i < d->items->idx; i++) {
			if(!d->items->arr[i])
				break;

			if(((file_t)d->items->arr[i])->is_file) {
				file_Destruct((file_t)d->items->arr[i]);
				continue;
			}

			dir_Destruct((dir_t)d->items->arr[i]);
		}
	}

	free(d);
}