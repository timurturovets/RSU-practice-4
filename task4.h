#include <stdlib.h>
#include <ctype.h>

typedef struct Student{
    unsigned int id;
    String name;
    String surname;
    String academic_group;
    unsigned char *grades;
    size_t grades_count;
    int is_initialized;
} Student, *p_student;

// tak nazyvaemoe
typedef enum Student_field {
    id,
    name,
    surname,
    group
} student_field;

typedef int (students_comparer) (const void*, const void*);

void print_menu();
int find_student_by(p_student array, size_t arr_len, p_student result, student_field key, void *value, int *is_found);
int order_students_by(p_student array, size_t arr_len, student_field key);
void print_student_to_console(p_student entity);
int print_student_to_file(char *file_path, p_student entity);
int print_higher_than_avg_GPAers(char *file_path, p_student array, size_t arr_len, float overall_avg);
int compare_students_by_id(const void *ent_1, const void *ent_2);
int compare_students_by_surname(const void *ent_1, const void *ent_2);
int compare_students_by_name(const void *ent_1, const void *ent_2);
int compare_students_by_group(const void *ent_1, const void *ent_2);
int get_avg_grade(p_student entity, float *result);
int get_overall_average(p_student array, size_t arr_len, float *result);

// tak nazyvaemiy
int student_constructor(p_student result, unsigned int id,
                        char *name, char *surname, char *group,
                        unsigned char *grades, size_t grades_count) {
    String str_name, str_surname, str_group;
    Student entity;

    if (name == NULL || surname == NULL || group == NULL || grades == NULL) return INVALID_PARAMETER;
    if (strlen(name) == 0 || strlen(surname) == 0 || strlen(group) == 0 || grades_count == 0) return INVALID_PARAMETER;

    entity.grades = (unsigned char *) malloc(sizeof(unsigned char) * grades_count);
    if (entity.grades == NULL) return MEMORY_ALLOCATION_ERROR;

    if (strlen(name) == 0 || !is_every_char_latin(name)
        || strlen(surname) == 0 || !is_every_char_latin(surname)
        || strlen(group) == 0) {
        free(entity.grades);
        return INVALID_INPUT;
    }

    string_constructor(&str_name, name);
    string_constructor(&str_surname, surname);
    string_constructor(&str_group, group);

    entity.id = id;
    entity.name = str_name;
    entity.surname = str_surname;
    entity.academic_group = str_group;
    memcpy(entity.grades, grades, grades_count * sizeof(unsigned char));
    entity.grades_count = grades_count;
    entity.is_initialized = 1;

    *result = entity;

    return OK;
}

int free_student(p_student entity) {
    if (entity == NULL) return INVALID_PARAMETER;

    clear_string(&entity->name);
    clear_string(&entity->surname);
    clear_string(&entity->academic_group);
    free(entity->grades);
    entity->is_initialized = 0;

    return OK;
}

int read_students_from_file(char const *file_path, p_student *result, int *count) {
    FILE *p_file;
    unsigned int curr_id;
    char curr_c, *curr_name, *curr_surname, *curr_group;
    int i, result_code;
    unsigned char *curr_grades;
    p_student p_res;

    if (file_path == NULL || count == NULL) return INVALID_PARAMETER;

    p_file = fopen(file_path, "r");

    if (p_file == NULL) return INVALID_INPUT;

    curr_name = (char*) malloc(BUFSIZ * sizeof(char));
    if (curr_name == NULL) {
        govnochist('f', p_file,
                   'm', result,
                   'q', NULL);

        return MEMORY_ALLOCATION_ERROR;
    }

    curr_surname = (char*) malloc(BUFSIZ * sizeof(char));
    if (curr_surname == NULL) {
        govnochist('f', p_file, 'm', result, 'm', curr_name, 'q', NULL);

        return MEMORY_ALLOCATION_ERROR;
    }

    curr_group = (char*) malloc(BUFSIZ * sizeof(char));
    if (curr_group == NULL) {
        govnochist('f', p_file, 'm', result, 'm', curr_name, 'm', curr_surname, 'q', NULL);

        return MEMORY_ALLOCATION_ERROR;
    }

    curr_grades = (unsigned char*) malloc(BUFSIZ * sizeof(int));
    if (curr_grades == NULL) {
        govnochist('f', p_file, 'm', result, 'm', curr_name, 'm', curr_surname, 'm', curr_group, 'q', NULL);

        return MEMORY_ALLOCATION_ERROR;
    }

    *count = 0;
    p_res = *result;
    while (!feof(p_file)) {
        fscanf(p_file, "%u %s %s %s", &curr_id, curr_name, curr_surname, curr_group);

        i = 0;
        while ((curr_c = fgetc(p_file)) != '\n' && !feof(p_file)) {
            if (isdigit(curr_c)) curr_grades[i++] = curr_c - '0';
        }

        result_code = student_constructor(p_res++,
                                          curr_id, curr_name, curr_surname, curr_group,
                                          curr_grades, i);

        if (result_code == OK) *count += 1;
        else {
            govnochist('f', p_file, 'm', curr_name, 'm', curr_surname, 'm', curr_group, 'm', curr_grades);

            return result_code;
        }
    }

    fclose(p_file);

    return OK;
}

int task_4(int argc, char **argv) {
    char *user_value;
    int count, user_choice, result_code, search_succeeded;
    float overall_avg_grade;
    Student search_result;
    p_student array, p_search_result = &search_result;
    student_field keys[4] = { id, surname, name, group };

    if (argc < 3) {
        PRINT_INVALID_INPUT_MESSAGE();
        return INVALID_INPUT;
    }

    argv++;

    if ((array = (p_student) malloc(BUFSIZ * sizeof(Student))) == NULL) {
        PRINT_MEMORY_ALLOCATION_ERROR();
        return MEMORY_ALLOCATION_ERROR;
    }

    if ((user_value = (char*) malloc(BUFSIZ * sizeof(char))) == NULL) {
        free(array);
        PRINT_MEMORY_ALLOCATION_ERROR();
        return MEMORY_ALLOCATION_ERROR;
    }

    result_code = read_students_from_file(*argv++, &array, &count);
    if (result_code != OK) {
        print_error(result_code);
        govnochist('m', array, 'm', user_value, 'q', NULL);
        return result_code;
    }

    get_overall_average(array, count, &overall_avg_grade);

    print_menu();
    do {
        printf("\nChoose option: ");
        scanf("%d", &user_choice);
        if (user_choice >= 1 && user_choice <= 4) {
            printf("Input key: ");
            scanf("%s", user_value);

            result_code = find_student_by(array, count, p_search_result,
                                          keys[user_choice - 1], user_value, &search_succeeded);
            if (result_code != OK) {
                print_error(result_code);
                govnochist('m', array, 'm', user_value, 'q', NULL);
                return result_code;
            }

            if (!search_succeeded) {
                printf("No record found!\n");
                continue;
            }

            printf("Found a record: \n");
            print_student_to_console(p_search_result);
        }

        else if (user_choice >= 5 && user_choice <= 8) {
            result_code = order_students_by(array, count, keys[user_choice - 5]);
            if (result_code != OK) {
                print_error(result_code);
                continue;
            }
            printf("Success!\n");
        }

        else if (user_choice == 9) {
            result_code = print_student_to_file(*argv, p_search_result);
            if (result_code != OK) {
                print_error(result_code);
                continue;
            }
            printf("Success!\n");
        }

        else if (user_choice == 10) {
            result_code = print_higher_than_avg_GPAers(*argv, array, count, overall_avg_grade);
            if (result_code != OK) {
                print_error(result_code);
                continue;
            }
            printf("Success!\n");
        }

        else printf("Invalid choice.\n");
    } while(user_choice != 0);

    printf("Success!");

    return OK;
}

void print_menu() {
    printf("\n0. Exit\n");
    printf("1. Search by ID;\n2. Search by surname;\n3. Search by name;\n4. Search by group;\n");
    printf("5. Sort by ID;\n6. Sort by surname;\n7. Sort by name;\n8. Sort by group;\n");
    printf("9. Print previously found student into the trace file;\n");
    printf("10. Print students with above average GPA.\n");
}

int find_student_by(p_student array, size_t arr_len, p_student result, student_field key, void *value, int *is_found) {
    if (array == NULL || result == NULL || value == NULL) return INVALID_PARAMETER;

    Student curr_entity;
    size_t i;
    int cmp_result;
    String str_value;
    if (key != id) string_constructor(&str_value, value);

    *is_found = 0;
    switch (key) {
        case id:
            for (i = 0; i < arr_len; i++) {
                curr_entity = array[i];
                if (curr_entity.id == atoi(value)) {
                    *result = curr_entity;
                    *is_found = 1;
                    return OK;
                }
            }

            break;

        case surname:
            for (i = 0; i < arr_len; i++) {
                curr_entity = array[i];
                string_cmp(&cmp_result, &curr_entity.surname, &str_value);
                if (cmp_result == 0) {
                    *result = curr_entity;
                    *is_found = 1;
                    return OK;
                }
            }

            break;

        case name:
            for (i = 0; i < arr_len; i++) {
                curr_entity = array[i];
                string_cmp(&cmp_result, &curr_entity.name, &str_value);
                if (cmp_result == 0) {
                    *result = curr_entity;
                    *is_found = 1;
                    return OK;
                }
            }

            break;

        case group:
            for (i = 0; i < arr_len; i++) {
                curr_entity = array[i];
                string_cmp(&cmp_result, &curr_entity.academic_group, &str_value);
                if (cmp_result == 0) {
                    *result = curr_entity;
                    *is_found = 1;
                    return OK;
                }
            }
            break;
        default:
            return INVALID_PARAMETER;
    }

    return OK;
}

int order_students_by(p_student array, size_t arr_len, student_field key) {
    if (array == NULL) return INVALID_PARAMETER;

    students_comparer* comparers[4] = {
            compare_students_by_id,
            compare_students_by_surname,
            compare_students_by_name,
            compare_students_by_group};

    qsort(array, arr_len, sizeof(Student), comparers[key]);
    return OK;
}

void print_student_to_console(p_student entity) {
    if (entity == NULL) return;

    int i;

    printf("Student named ");
    print_string(&entity->name, ' ');
    print_string(&entity->surname, '\n');

    printf("ID: %d\n", entity->id);
    printf("Academic group: ");
    print_string(&entity->academic_group, '\n');

    printf("Grades: ");
    for (i = 0; i < entity->grades_count; i++) {
        printf("%d%s", entity->grades[i], i == entity->grades_count - 1 ? "\n" : ", ");
    }
}

int print_student_to_file(char *file_path, p_student entity) {
    FILE* p_file = fopen(file_path, "a");
    if (p_file == NULL) return INVALID_PARAMETER;

    int i;
    float avg_grade;

    fprintf(p_file, "Student named ");
    fprint_string(p_file, &entity->name, ' ');
    fprint_string(p_file, &entity->surname, '\n');

    fprintf(p_file, "Academic group: ");
    fprint_string(p_file, &entity->academic_group, '\n');

    fprintf(p_file, "Grades: ");
    for (i = 0; i < entity->grades_count; i++) {
        fprintf(p_file, "%d%s", entity->grades[i], i == entity->grades_count - 1 ? "; " : ", ");
    }

    get_avg_grade(entity, &avg_grade);
    fprintf(p_file, "Average: %f\n", avg_grade);

    fclose(p_file);

    return OK;
}

int print_higher_than_avg_GPAers(char *file_path, p_student array, size_t arr_len, float overall_avg) {
    if (file_path == NULL || array == NULL) return INVALID_PARAMETER;

    int i;
    float curr_avg;

    for (i = 0; i < arr_len; i++) {
        get_avg_grade(array + i, &curr_avg);
        if (curr_avg > overall_avg) {
            print_student_to_file(file_path, array + i);
        }
    }

    return OK;
}

int compare_students_by_id(const void *ent_1, const void *ent_2) {
    p_student value_1 = (p_student)ent_1, value_2 = (p_student)ent_2;

    return value_1->id - value_2->id;
}

int compare_students_by_surname(const void *ent_1, const void *ent_2) {
    p_student value_1 = (p_student)ent_1, value_2 = (p_student)ent_2;

    int result;
    string_cmp(&result, &value_1->surname, &value_2->surname);
    return result;
}

int compare_students_by_name(const void *ent_1, const void *ent_2) {
    p_student value_1 = (p_student)ent_1, value_2 = (p_student)ent_2;

    int result;
    string_cmp(&result, &value_1->name, &value_2->name);
    return result;
}

int compare_students_by_group(const void *ent_1, const void *ent_2) {
    p_student value_1 = (p_student)ent_1, value_2 = (p_student)ent_2;

    int result;
    string_cmp(&result, &value_1->academic_group, &value_2->academic_group);
    return result;
}

int get_avg_grade(p_student entity, float *result) {
    if (entity == NULL || result == NULL) return INVALID_PARAMETER;

    int i, sum;
    for (i = 0, sum = 0; i < entity->grades_count; i++) {
        sum += entity->grades[i];
    }

    *result = sum / (float)entity->grades_count;

    return OK;
}

int get_overall_average(p_student array, size_t arr_len, float *result) {
    if (array == NULL || result == NULL) return INVALID_PARAMETER;

    size_t i;
    float avg = 0.0f, curr_avg;
    for (i = 0; i < arr_len; i++) {
        get_avg_grade(array + i, &curr_avg);
        avg += curr_avg;
    }

    avg /= (float)arr_len;
    *result = avg;

    return OK;
}