#define STRING_CLIBP
#include "../src/clibp.h"

str_t str_Replace(str_t s, const str find, const str replacement) {
    if(!s || !find)
        return NULL;

    str_t n = new_str(NULL, 0);
    int pos = 0, last = 0, match = 0;
    while((pos = str_FindString(s, find, match)) != -1) {
        str sub = str_GetSub(s, last, pos);
        if(sub != NULL) {
            str_t _sub = new_str(sub, 0);
            str_Append(n, _sub);
            if(replacement != NULL)
                str_Append(n, new_str(replacement, 1));
            
            str_Destruct(_sub);
        }
        match++;
        last = pos + strlen(find);
    }

    if((last + strlen(find)) != s->idx) {
        str sub = str_GetSub(s, last, s->idx);
        str_Append(n, new_str(sub, 0));
        free(sub);
    }
    
    return n;
}

void test_status_msg(int test_expr, char *msg) {
    printf("[ \x1b[32mTEST\x1b[0m ] Status: %d | %s", test_expr, msg);
}

int main() {
    str_t hello = new_str(NULL, 0);
    str_Append(hello, new_str("Hello ", 1));
    str_Append(hello, new_str("World! ", 1));
    str_Append(hello, new_str("World! ", 1));
    str_Append(hello, new_str("grggWorld!gtgdgeg", 1));

    printf("%s | Idx: %d\n", hello->data, hello->idx);

    str_t n = str_Replace(hello, "World", NULL);

    str_t buff = new_str(strdup("Replace(...): "), 0);
    str_Append(buff, n);
    str_Append(buff, new_str(" | Idx: ", 1));
    str_iAppend(buff, n->idx);
    str_Append(buff, new_str("\n", 1));
    test_status_msg(strcmp(hello->data, n->data) ? 1 : 0, buff->data);
    
    str_Destruct(buff);
    str_Destruct(n);
    str_Destruct(hello);
    return 0;
}