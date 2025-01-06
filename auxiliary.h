#pragma once

#include <stdarg.h>
#include <ctype.h>

#define OK 0
#define ERR (-1)
#define INVALID_INPUT (-2)
#define INVALID_PARAMETER (-3)
#define MEMORY_ALLOCATION_ERROR (-4)

#define PRINT_ERROR_MESSAGE() printf("Error: unforeseen error. Restarting the program may help.")
#define PRINT_INVALID_INPUT_MESSAGE() printf("Error: invalid input. Please restart the program and pass proper input values.")
#define PRINT_INVALID_PARAMETER_MESSAGE() printf("Error: invalid parameter. Restarting the program may help.")
#define PRINT_MEMORY_ALLOCATION_ERROR() printf("Error: couldn't allocate memory. Restarting the program may help.")

int govnochist(int flag, ...) {
    va_list p_args;
    if (flag != 'f' && flag != 'm') return INVALID_PARAMETER;

    va_start(p_args, flag);

    while (1) {
        void* resource = va_arg(p_args, void*);

        if(resource == NULL) {
            va_end(p_args);
            return OK;
        }

        if (flag == 'm') free(resource);
        else if (flag == 'f') fclose(resource);
        else {
            va_end(p_args);
            return INVALID_PARAMETER;
        }

        flag = va_arg(p_args, int);
    }
}

int ctod(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    else if (c >= 'A' && c <= 'Z') return 10 + (c - 'A');
    else return -1;
}

char dtoc(int digit) {
    if (digit >= 0 && digit <= 9) return '0' + digit;
    else if (digit >= 10 && digit <= 35) return 'A' + (digit - 10);
    else return -1;
}

int is_latin(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int is_every_char_latin(char* str) {
    size_t i, len;
    for (i = 0, len = strlen(str); i < len; i++) {
        if (!is_latin(str[i])) return 0;
    }
    return 1;
}

int is_every_char_digit(char *str) {
    int i, len;
    for (i = 0, len = strlen(str); i < len; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int is_valid_datetime_str(char *str) {
    // 0123456789 10 11 12 13 14 15 16 17 18
    // dd:MM:yyyy h  h  :  m   m  :  s s
    int d, M, y, h, m, s;
    if (strlen(str) != 19 || str[2] != ':' || str[5] != ':' || str[13] != ':' || str[16] != ':') return 0;

    // validate days
    if (ctod(str[0]) != '0') {

    }
    return 1;
}

void print_error(int error_code) {
    switch (error_code) {
        case INVALID_INPUT:
            PRINT_INVALID_INPUT_MESSAGE();
            break;
        case INVALID_PARAMETER:
            PRINT_INVALID_PARAMETER_MESSAGE();
            break;
        case MEMORY_ALLOCATION_ERROR:
            PRINT_MEMORY_ALLOCATION_ERROR();
            break;
        case ERR:
            PRINT_ERROR_MESSAGE();
            break;
        default:
            break;
    }
}