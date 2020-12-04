#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Task {
    float util;
    float runtime;
    unsigned int period;
} Task;

typedef struct TaskSet {
    unsigned int num_tasks;
    Task* tasks;
} TaskSet;

// Reads specified input file for generated task sets
// Places read task sets and their tasks into array of TaskSet objects
// Each TaskSet will contain an array of Task objects
// Remember!: You must manually free the TaskSet array and the Task array
// in each TaskSet once you're done using it
void read_tasksets(char* file_name, TaskSet** ts1, unsigned int* num_sets) {

    TaskSet* ts = NULL;
    FILE * fp;
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("Could not open file: %s\n", file_name);
        ts = NULL;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    char* pt;
    unsigned int set_count = 0;
    unsigned int task_count = 0;

    // Count number of sets
    *num_sets = 0;
    getline(&line, &len, fp); // Throw away the first line (column headers)
    while ((read = getline(&line, &len, fp)) != -1) {
        if (*line == '\n') {
            *num_sets += 1;
        }
    }
    rewind(fp);

    // Now that we know the number of task sets, we can make an array
    // to hold all the sets' tasks.
    ts = (TaskSet*) malloc(*num_sets * sizeof(TaskSet));
    for (unsigned int i = 0; i < *num_sets; i++) {
        ts[i].num_tasks = 0;
    }

    // Count how many tasks are in each set
    getline(&line, &len, fp); // Throw away the first line (column headers)
    set_count = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        if (*line == '\n') {
            set_count++;
            continue;
        }
        ts[set_count].num_tasks++;
    }
    rewind(fp);

    // Allocate each set's task array
    for (unsigned int i = 0; i < *num_sets; i++) {
        ts[i].tasks = (Task*) malloc(ts[i].num_tasks * sizeof(Task));
    }

    // Get the data
    getline(&line, &len, fp); // Throw away the first line (column headers)
    set_count = 0;
    task_count = 0;
    while ((read = getline(&line, &len, fp)) != -1) {

        // If it's a blank line, there's no data to fetch, so continue
        // This indicates a new set will begin on the next line
        if (*line == '\n') {
            if (task_count != ts[set_count].num_tasks) {
                printf("Bug in grabbing data!!!!");
            }
            set_count++;
            task_count = 0;
            continue;
        }

        // Split up the line's 3 pieces of data by comma
        pt = strtok (line,",");
        ts[set_count].tasks[task_count].runtime = atof(pt);

        pt = strtok (NULL, ",");
        ts[set_count].tasks[task_count].period = atoi(pt);

        pt = strtok (NULL, ",");
        ts[set_count].tasks[task_count].util = atof(pt);

        task_count++;
    }
    rewind(fp);

    fclose(fp);
    if (line)
        free(line);

    *ts1 = ts;
}