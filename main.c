#include <stdio.h>
#include "auxiliary.h"

#include "task1.h"

typedef int (task_func) (int, char**);

int main(int argc, char** argv) {
    task_func* tasks[1] = {task_1};

    int choice;
    printf("Choose task: ");
    scanf("%d", &choice);

    tasks[choice-1](argc, argv);
}
