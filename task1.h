#pragma once

#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
typedef enum sort_direction {
    ascending,
    descending
} sort_direction;

int compare_asc(const void * item_1, const void * item_2);
int compare_desc(const void * item_1, const void * item_2);

typedef struct Employee {
    unsigned int id;
    char *name;
    char *surname;
    double salary;
} employee, *p_employee;

int is_latin(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

//  tak nazyvaemiy
int employee_constructor (p_employee dest, unsigned int id, char *name, char *surname, double salary) {
    if (dest == NULL || name == NULL || surname == NULL) return INVALID_PARAMETER;

    if (salary < 0) return INVALID_INPUT;

    size_t i, len_name = strlen(name), len_surname = strlen(surname);
    if (len_name == 0 || len_surname == 0) return INVALID_INPUT;

    for (i = 0; i < len_name; i++) {
        if (!is_latin(name[i])) return INVALID_INPUT;
    }

    for (i = 0; i < len_surname; i++) {
        if (!is_latin(surname[i])) return INVALID_INPUT;
    }

    dest->name = (char*) malloc (BUFSIZ * sizeof(char));
    if (dest->name == NULL) return MEMORY_ALLOCATION_ERROR;

    dest->surname = (char*) malloc(BUFSIZ*sizeof(char));
    if (dest->surname == NULL) {
        free(dest->name);

        return MEMORY_ALLOCATION_ERROR;
    }
    dest->id = id;
    strcpy(dest->name, name);
    strcpy(dest->surname, surname);
    dest->salary = salary;

    return OK;
}

int task_1(int argc, char** argv) {
    if (argc < 4) return INVALID_PARAMETER;

    char *curr_name, *curr_surname,
        *input_file_path = NULL,
        *flag = NULL,
        *output_file_path = NULL;
    unsigned int i, count, curr_id, result_code;
    sort_direction sort_dir;
    float curr_salary;
    p_employee employees;

    argv++;
    input_file_path = *argv++;
    flag = *argv++;
    output_file_path = *argv;

    if (input_file_path == NULL || flag == NULL || output_file_path == NULL) return INVALID_PARAMETER;

    if (strcmp(flag, "-a") == 0 || strcmp(flag, "/a") == 0) sort_dir = ascending;
    else if (strcmp(flag, "-d") == 0 || strcmp(flag, "/d") == 0) sort_dir = descending;
    else return INVALID_PARAMETER;

    FILE *p_input_file = fopen(input_file_path, "r");
    if (p_input_file == NULL) return INVALID_PARAMETER;

    FILE *p_output_file = fopen(output_file_path, "w");
    if (p_output_file == NULL) {
        fclose(p_input_file);
        return INVALID_PARAMETER;
    }

    employees = (p_employee) malloc(sizeof(employee) * 16);

    if (employees == NULL) {
        fclose(p_input_file);
        fclose(p_output_file);

        return MEMORY_ALLOCATION_ERROR;
    }

    curr_name = (char*) malloc(BUFSIZ * sizeof(char));
    if (curr_name == NULL) {
        fclose(p_input_file);
        fclose(p_output_file);
        free(employees);

        return MEMORY_ALLOCATION_ERROR;
    }

    curr_surname = (char*) malloc(BUFSIZ * sizeof(char));
    if (curr_surname == NULL) {
        fclose(p_input_file);
        fclose(p_output_file);
        free(employees);
        free(curr_name);

        return MEMORY_ALLOCATION_ERROR;
    }

    p_employee p_employees = employees;
    count = 0;
    while (!feof(p_input_file)) {
        fscanf(p_input_file, "%u %s %s %f", &curr_id, curr_name, curr_surname, &curr_salary);

        result_code = employee_constructor(p_employees++, curr_id, curr_name, curr_surname, curr_salary);
        if (result_code != OK) {
            fclose(p_input_file);
            fclose(p_output_file);
            free(employees);

            return result_code;
        }
        count++;
    }

    if (sort_dir == ascending) qsort(employees, count, sizeof(employee), compare_asc);
    else qsort(employees, count, sizeof(employee), compare_desc);

    for (i = 0; i < count; i++) {
        fprintf(p_output_file, "%u %s %s %f\n",
                employees[i].id, employees[i].name, employees[i].surname, employees[i].salary);
    }

    fclose(p_input_file);
    fclose(p_output_file);

    printf("Success!");
    return OK;
}

int compare_asc(const void * item_1, const void * item_2) {
    p_employee p_emp1 = (p_employee)item_1,
               p_emp2 = (p_employee)item_2;

    if (p_emp1->salary > p_emp2->salary) return 1;
    if (p_emp1->salary < p_emp2->salary) return -1;

    int surname_cmpres = strcmp(p_emp1->surname, p_emp2->surname);
    if (surname_cmpres > 0) return 1;
    if (surname_cmpres < 0) return -1;

    int name_cmpres = strcmp(p_emp1->name, p_emp2->name);
    if (name_cmpres > 0) return 1;
    if (name_cmpres < 0) return -1;

    return p_emp1->id > p_emp2->id;
}

int compare_desc(const void * item_1, const void * item_2) {
    p_employee p_emp1 = (p_employee)item_1,
            p_emp2 = (p_employee)item_2;

    if (p_emp1->salary > p_emp2->salary) return -1;
    if (p_emp1->salary < p_emp2->salary) return 1;

    int surname_cmpres = strcmp(p_emp1->surname, p_emp2->surname);
    if (surname_cmpres > 0) return -1;
    if (surname_cmpres < 0) return 1;

    int name_cmpres = strcmp(p_emp1->name, p_emp2->name);
    if (name_cmpres > 0) return -1;
    if (name_cmpres < 0) return 1;

    return p_emp1->id > p_emp2->id;
}

// проверить все маллоки
// проверить данные с командной строки
// пацанская сортировка
// запись в файл по людски