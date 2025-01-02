#include <stdlib.h>
#include <ctype.h>

void print_menu();

typedef struct Student{
    unsigned int id;
    String name;
    String surname;
    String academic_group;
    unsigned char *grades;
    size_t grades_count;
} Student, *p_student;

// tak nazyvaemiy
int student_constructor(p_student result, unsigned int id,
                        char *name, char *surname, char *group,
                        unsigned char *grades, size_t grades_count) {
    if (name == NULL || surname == NULL || group == NULL || grades == NULL) return INVALID_PARAMETER;
    if (strlen(name) == 0 || strlen(surname) == 0 || strlen(group) == 0 || grades_count == 0) return INVALID_PARAMETER;

    Student entity;

    entity.grades = (unsigned char *) malloc(sizeof(unsigned char) * grades_count);
    if (entity.grades == NULL) return MEMORY_ALLOCATION_ERROR;

    if (strlen(name) == 0 || !is_every_char_latin(name)
        || strlen(surname) == 0 || !is_every_char_latin(surname)
        || strlen(group) == 0) {
        free(entity.grades);
        return INVALID_INPUT;
    }

    String str_name, str_surname, str_group;
    string_constructor(&str_name, name);
    string_constructor(&str_surname, surname);
    string_constructor(&str_group, group);

    entity.id = id;
    entity.name = str_name;
    entity.surname = str_surname;
    entity.academic_group = str_group;
    memcpy(entity.grades, grades, grades_count * sizeof(unsigned char));
    entity.grades_count = grades_count;

    *result = entity;
    return OK;
}

int read_students_from_file(char const *file_path, p_student *result, int *count) {
    if (file_path == NULL || count == NULL) return INVALID_PARAMETER;

    unsigned int curr_id;
    char curr_c, *curr_name, *curr_surname, *curr_group;
    int i, result_code;
    unsigned char *curr_grades;
    p_student p_res;

    FILE *p_file = fopen(file_path, "r");
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

    return OK;
}

int task_4(int argc, char **argv) {
    if (argc < 3) {
        PRINT_INVALID_INPUT_MESSAGE();
        return INVALID_INPUT;
    }

    argv++;

    int i, count, user_choice;
    p_student array;

    if ((array = (p_student) malloc(BUFSIZ * sizeof(Student))) == NULL) {
        PRINT_MEMORY_ALLOCATION_ERROR();
        return MEMORY_ALLOCATION_ERROR;
    }


    puts(*++argv);
    read_students_from_file(*argv, &array, &count);

//    printf("\n\ncount: %d\n", count);
//    for (i = 0; i < count; i++) {
//        printf("\nStudent %d %lld: ", array[i].id, array[i].grades_count);
//        print_string(&array[i].name, ' ');
//        for (int j = 0; j < array[i].grades_count; j++) {
//            printf("%d ", array[i].grades[j]);
//        }
//    }

    do {
        print_menu();
        printf("Choose option: ");
        scanf("%d", &user_choice);
    } while(user_choice != 0);

    printf("Success!");

    return OK;
}

void print_menu() {
    printf("\n0. Exit\n");
    printf("1. Search by ID;\n2. Search by surname;\n3. Search by name;\n4. Search by group;\n");
    printf("5. Sort by ID;\n6. Sort by surname;\n7. Sort by name;\n8.Sort by group.\n");

}