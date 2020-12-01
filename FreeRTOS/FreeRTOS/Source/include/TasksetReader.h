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
    unsigned int num_sets_alloced = 10;
    unsigned int num_tasks_alloced [10];
    unsigned int t;

    // Initially we'll make the lists 10 items long
    // The lists will be resized dynamically as we read in the file
    ts = (TaskSet*) malloc(10*sizeof(TaskSet));
    for(unsigned int i = 0; i < 10; i++) {
        ts[i].num_tasks = 0;
        ts[i].tasks = (Task*) malloc(10*sizeof(Task));
        num_tasks_alloced[i] = 10;
    }

    // Read in the file, line by line
    getline(&line, &len, fp); // Throw away the first line (column headers)
    while ((read = getline(&line, &len, fp)) != -1) {

        if (strlen(line)==1) {
            set_count++;
            continue;
        }

        // Split the line up by the comma delimiters
        pt = strtok (line,",");
        t = ts[set_count].num_tasks;

        // Check if TaskSet array is full and needs resizing
        if (set_count == num_sets_alloced) {
            ts = realloc(ts, 2*num_sets_alloced*sizeof(TaskSet));
            num_sets_alloced *= 2;
        }

        // Check if Task array is full and needs resizing
        if (t == num_tasks_alloced[set_count]) {
            ts[set_count].tasks = realloc(ts[set_count].tasks, 2*t*sizeof(Task));
            num_tasks_alloced[set_count] *= 2;
        }

        ts[set_count].tasks[t].runtime = atof(pt);
        pt = strtok (NULL, ",");

        ts[set_count].tasks[t].period = atoi(pt);
        pt = strtok (NULL, ",");

        ts[set_count].tasks[t].util = atof(pt);

        ts[set_count].num_tasks++;
    }

    fclose(fp);
    if (line)
        free(line);

    *num_sets = set_count;
    *ts1 = ts;
}