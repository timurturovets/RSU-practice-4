

typedef struct String {
    char *inner_array;
    int len;
} String, *p_string;

// tak nazyvaemiy
int string_constructor(p_string dest, char *src) {
    if (src == NULL) return INVALID_PARAMETER;

    char *inner_array;
    int i, len = strlen(src);
    if (len == 0) return INVALID_INPUT;

    inner_array = (char*) malloc((len-1) * sizeof(char));
    if (inner_array == NULL) return MEMORY_ALLOCATION_ERROR;

    for (i = 0; i < len; i++) {
        inner_array[i] = src[i];
    }

    String entity;

    entity.inner_array = inner_array;
    entity.len = len;

    *dest = entity;

    return OK;
}

int task_2(int argc, char** argv) {
    char *text = "lorem ipsum";

    String str;
    string_constructor(&str, text);

    printf("Str len: %d\n", str.len);
    printf("Str: ");
    for (int i = 0; i < str.len; i++) {
        printf("%c", str.inner_array[i]);
    }

}