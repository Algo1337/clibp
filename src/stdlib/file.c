#define CLIBP
#define STRING_CLIBP
#define ARRAY_CLIBP
#include "../../headers/clibp.h"

File *open_file(const char *filename, const char *mode) {
    if(!filename || !mode)
        return NULL;

    File *f = (File *)malloc(sizeof(File));
    f->filename = strdup(filename);
    f->content = NULL;
    if(!(f->stream = fopen(filename, mode))) {
        free(f);
        return NULL;
    }

    return f;
}

int file_Read(File *f) {
    if(!f)
        return 0;

    fseek(f->stream, 0L, SEEK_END);
    f->idx = ftell(f->stream);
    fseek(f->stream, 0L, SEEK_SET);

    f->content = (char *)malloc(f->idx);
    memset(f->content, 0, f->idx);
    size_t bytes_r = fread(f->content, 1, f->idx, f->stream);
    if(bytes_r < 0)
        return 0;

    return bytes_r;
}

int file_Write(File *f, const char *data) {
    if(!f)
        return 0;

    fwrite(data, 1, strlen(data), f->stream);
    fflush(f->stream);

    return 1;
}

int file_ReadBinary(File *f) {
    if(!f)
        return 0;
}

void file_Destruct(File *f) {
    if(!f)
        return;

    if(f->stream)
        fclose(f->stream);

    if(f->content)
        free(f->content);

    free(f);
}