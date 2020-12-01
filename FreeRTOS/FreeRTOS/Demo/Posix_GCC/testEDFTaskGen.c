#include "../../Source/include/TasksetReader.h"
#include <stdio.h>

void main_taskgen (char* filename) {

    TaskSet* taskSets = NULL;
    unsigned int num_sets;
    read_tasksets(filename, &taskSets, &num_sets);

    for (unsigned int i = 0; i < num_sets; i++) {
        printf("=============Task Set #%d=============\n", i+1);
        for (unsigned int j = 0; j < taskSets[i].num_tasks; j++) {
            printf("Runtime: %f, Period: %d, Utilization: %f\n",
                   taskSets[i].tasks[j].runtime,
                   taskSets[i].tasks[j].period,
                   taskSets[i].tasks[j].util);
        }
    }

    for (unsigned int i = 0; i < num_sets; i++) {
        free(taskSets[i].tasks);
    }
    free(taskSets);
}