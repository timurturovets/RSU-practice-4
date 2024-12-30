#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>

#include "auxiliary.h"
#include "task1.h"
#include "task2.h"

typedef int (task_func) (int, char**);

int main(int argc, char** argv) {
    task_func* tasks[2] = {task_1, task_2};

    int choice;
    printf("Choose task: ");
    scanf("%d", &choice);

    tasks[choice-1](argc, argv);
}
