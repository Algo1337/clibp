#pragma once

#include <dirent.h>
#include <sys/stat.h>

#include "str.h"
#include "arr.h"

typedef struct File {
    char *filename;
    char *content;
    size_t idx;
    FILE *stream;
} File;

typedef File *file_t;

File *open_file(const char *filename, const char *mode);
int file_Read(File *f);
int file_Write(File *f, const char *data);
void file_Destruct(File *f);