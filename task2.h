#pragma once


#include "auxiliary.h"

typedef struct String {
    char *value;
    int len;
} String, *p_string;

int string_constructor(p_string dest, char *src);
int clear_string(p_string str);
int string_cmp(int *result, p_string str_1, p_string str_2);
int string_cpy(p_string dest, p_string src);
int make_str_from_source(p_string result, p_string src);
int string_concat(p_string dest, p_string src);
void deep_copy(char *dest, char *src, size_t start, size_t end, size_t manip_ind_1, size_t manip_ind_2);

int task_2(int argc, char **argv) {
    int i, result;
    String str_1, str_2, str_3, str_4;

    char *text_1 = "lorem ipsum",
            *text_2 = "dolor sit amet",
            *text_3 = "lorem ipsum",
            *text_4 = "l1rem ipsum";

    string_constructor(&str_1, text_1);
    string_constructor(&str_2, text_2);
    string_constructor(&str_3, text_3);
    string_constructor(&str_4, text_4);

    printf("Str: ");
    for (i = 0; i < str_1.len; i++) {
        printf("%c", str_1.value[i]);
    }
    printf("\nStr len: %d\n", str_1.len);

    string_cmp(&result, &str_1, &str_2);
    printf("Comparing result 1: %d\n", result);

    string_cmp(&result, &str_1, &str_3);
    printf("Comparing result 2: %d\n", result);

    string_cmp(&result, &str_3, &str_4);
    printf("Comparing result 3: %d\n", result);

    clear_string(&str_2);
    printf("String len after clearing: %d\n", str_2.len);

    string_concat(&str_1, &str_4);
    printf("After concat:");
    for (i = 0; i < str_1.len; i++) {
        printf("%c", str_1.value[i]);
    }

    return OK;
}

// tak nazyvaemiy
int string_constructor(p_string dest, char *src) {
    String entity;
    char *inner_array;
    int i, len;

    if (src == NULL) return INVALID_PARAMETER;

    len = strlen(src);

    inner_array = (char*) malloc((len-1) * sizeof(char));
    if (inner_array == NULL && len != 0) return MEMORY_ALLOCATION_ERROR;

    deep_copy(inner_array, src, 0, len, 0, 0);

    entity.value = inner_array;
    entity.len = len;

    *dest = entity;

    return OK;
}

int clear_string(p_string str) {
    if (str == NULL) return INVALID_PARAMETER;

    str->len = 0;
    if (str->value != NULL) free(str->value);
    str->value = NULL;

    return OK;
}

// otnoshenie poryadka i equivalentnost v odnom...
int string_cmp(int *result, p_string str_1, p_string str_2) {
    int i, len;

    if (str_1 == NULL || str_2 == NULL) return INVALID_PARAMETER;

    if ((*result = str_1->len - str_2->len)) return OK;

    for (i = 0, len = str_1->len; i < len; i++) {
        if ((*result = str_1->value[i] - str_2->value[i])) return OK;
    }

    *result = 0;

    return OK;
}

int string_cpy(p_string dest, p_string src) {
    int i, len = src->len;

    if (dest == NULL || src == NULL) return INVALID_PARAMETER;

    clear_string(dest);

    dest->value = (char*) malloc(src->len * sizeof(char));
    if (dest->value == NULL) return MEMORY_ALLOCATION_ERROR;

    deep_copy(dest->value, src->value, 0, len, 0, 0);
    dest->len = len;

    return OK;
}

int make_str_from_source(p_string result, p_string src) {
    int i, len = src->len;
    String new_str;

    if (src == NULL) return INVALID_PARAMETER;

    new_str.value = (char*) malloc(len * sizeof(char));
    if (new_str.value == NULL) return MEMORY_ALLOCATION_ERROR;

    new_str.len = len;

    deep_copy(new_str.value, src->value, 0, len, 0, 0);
    *result = new_str;

    return OK;
}

int string_concat(p_string dest, p_string src) {
    char* old_buf;
    int i, new_len;

    if (dest == NULL || src == NULL) return INVALID_PARAMETER;

    old_buf = dest->value;
    new_len = dest->len + src->len;
    dest->value = (char*) realloc(dest->value, new_len * sizeof(char));
    if (dest->value == NULL) {
        free(old_buf);

        return MEMORY_ALLOCATION_ERROR;
    }

    deep_copy(dest->value, src->value, dest->len, new_len, 0, -dest->len);
    dest->len = new_len;

    return OK;
}

void print_string(p_string str, char end) {
    int i;

    for (i = 0; i < str->len; i++) {
        printf("%c", str->value[i]);
    }

    printf("%c", end);
}

void fprint_string(FILE* dest, p_string str, char end) {
    int i;

    if (dest == NULL) return;

    for (i = 0; i < str->len; i++) {
        fprintf(dest, "%c", str->value[i]);
    }

    fprintf(dest, "%c", end);
}

void deep_copy(char *dest, char *src, size_t start, size_t end, size_t manip_ind_1, size_t manip_ind_2) {
    size_t i;
    for (i = start; i < end; i++) {
        dest[i + manip_ind_1] = src[i + manip_ind_2];
    }
}