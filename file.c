#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CLIBP
#include <clibp.h>

#include <dirent.h>
#include <sys/stat.h>

typedef arr_t items_t;

typedef struct Item {
	str_t 	name;
	size_t 	size;
	size_t 	is_file;
} Item;

typedef Item *file_t;

file_t create_file(str_t name, size_t sz) {
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

typedef arr_t items_t;

typedef struct Dir {
	str_t 	name;
	items_t items;
} Dir;

typedef Dir *dir_t;
typedef Item *item_t;

dir_t create_dir(str_t name) {
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

dir_t open_dir(str_t name) {
	Dir *d = (Dir *)malloc(sizeof(Dir));

	d->name = name;
	d->items = new_arr(NULL, 0);

	__get_dir_info__(d);
	return d;
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

int main() {
	str_t dir_name = new_str(strdup("."), 0);
	dir_t dir = open_dir(dir_name);

	for(int i = 0; i < dir->items->idx; i++) {
		if(!dir->items->arr[i])
			break;

		file_t item = (file_t)dir->items->arr[i];
		if(item->is_file) {
			printf("File: %s\n", item->name->data);
		}
	}

	dir_Destruct(dir);
	return 0;
}
