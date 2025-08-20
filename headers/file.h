#pragma once

#include <dirent.h>
#include <sys/stat.h>

#include "str.h"
#include "arr.h"

typedef arr_T *items_t;

typedef struct Item {
	str_T   *name;
	size_t 	size;
	size_t 	is_file;
} Item;

typedef struct Dir {
	str_T *	name;
	items_t items;
} Dir;

typedef Item *file_t;
typedef Dir *dir_t;

dir_t create_dir(str_T *name);
static void __get_dir_info__(dir_t d);
dir_t open_dir(str_T *name);
void dir_Destruct(dir_t d);

file_t create_file(str_T *name, size_t sz);
void file_Destruct(file_t file);