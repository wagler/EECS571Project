import csv

def write_task_func(set_num,task_num,rel_deadline, avg_exe_time):

    srccode= "//Task set #{}, Task #{}\n".format(set_num,task_num)
    srccode+= "//Deadline: {}, Avg Exe Time: {}\n".format(rel_deadline, avg_exe_time)
    srccode+="""static void T{}_{}(void *pvParameters)""".format(set_num,task_num) + " {"
    srccode+="""
    unsigned int job = 0;
    unsigned long end;
    for(;;)
    {   
        job++;
    """
    srccode+= "\tend = (xTaskGetTickCount() / {}) * {} + {};\n".format(rel_deadline,rel_deadline,avg_exe_time)
    srccode+= """\t\tprintf("Executing T{} start: %lu deadline: %d job: %d \\n", xTaskGetTickCount(), ((xTaskGetTickCount() / {}) + 1) * {}, job);""".format(task_num,rel_deadline,rel_deadline)
    srccode+="""
        while(xTaskGetTickCount() < end) { }

        vTaskDelay(1);
    }"""
    srccode+="\n}\n\n"

    return srccode

def write_task_funcs(sets):
    out = ""
    for set_num,taskset in enumerate(sets):
        for task_num,task in enumerate(taskset):
            prog = write_task_func(set_num,task_num, task[1], int(float(task[0])))
            out += prog
            out += "\n"
    return out

def write_task_handles(sets):
    out = "xTaskHandle "
    for set_num,taskset in enumerate(sets):
        for task_num in range(len(taskset)):
            out += "xT{}_{}".format(set_num,task_num)
            if not (task_num==(len(taskset)-1) and set_num==(len(sets)-1)):
                out += ", "
    out += ";\n\n"
    return out

def write_file_header():
    out = """#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h" \n\n\n"""
    return out

def read_tasksets_file(filename):
    sets = []
    curr_set = []
    set_count = 0
    with open(filename, newline='') as f:
        reader = csv.reader(f)
        next(reader)
        for row in reader:
            if (len(row)==0):
                sets.append(curr_set)
                curr_set = []
                continue
            curr_set.append(row)
    return sets

text_file = open("sample.h", "a")

sets = read_tasksets_file("tasksets.txt")

file_header = write_file_header()
text_file.write(file_header)

task_handles = write_task_handles(sets)
text_file.write(task_handles)

task_functions = write_task_funcs(sets)
text_file.write(task_functions)

text_file.close()

