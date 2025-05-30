#define STRING_CLIBP
#include "../src/clibp.h"

str_t str_Replace(str_t s, const str find, const str replacement) {
    if(!s)
        return NULL;

    str_t n = new_str(NULL, 0);
    int pos = 0, last = 0, match = 0;
    while((pos = str_FindString(s, find, match)) != -1) {
        str sub = str_GetSub(s, last, pos);
        str_Append(n, new_str(sub, 0));
        str_Append(n, new_str(replacement, 1));
        printf("%s\n", sub);
        free(sub);
        match++;
        last = pos + strlen(find);
    }
    
    return n;
}

int main() {
    str_t hello = new_str(NULL, 0);
    str_Append(hello, new_str("Hello ", 1));

    printf("%s\n", hello->data);

    int pos = str_FindString(hello, "World", 0);
    str_t n = str_Replace(hello, "World", "gay");
    printf("%s\n", n->data);
    return 0;
}