#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>

#include "auxiliary.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"

typedef int (task_func) (int, char**);

int main(int argc, char** argv) {
    task_func* tasks[4] = {task_1, task_2, task_3, task_4};

    int choice;
    printf("Choose task: ");
    scanf("%d", &choice);

    tasks[choice-1](argc, argv);
}
