#include <stdarg.h>

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
    if (flag != 'f' && flag != 'm') return INVALID_PARAMETER;

    va_list p_args;
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