import csv
import re
import sys

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
    srccode+= """\t\tprintf("Executing T{} start: %lu deadline: %ld job: %d \\n", xTaskGetTickCount(), ((xTaskGetTickCount() / {}) + 1) * {}, job);""".format(task_num,rel_deadline,rel_deadline)
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

def write_task_func_headers(sets):
    out = ""
    for set_num,taskset in enumerate(sets):
        for task_num,task in enumerate(taskset):
            out += "static void T{}_{}(void *pvParameters);\n".format(set_num,task_num)
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

def write_task_deadlines(sets):
    out = ""
    temp = ""
    for set_num,taskset in enumerate(sets):
        for task_num, task in enumerate(taskset):
            temp = "const unsigned long d{}_{}".format(set_num,task_num)
            temp += " = {};\n".format(task[1]) #set the deadline
            out += temp
    out += "\n"
    return out

def write_task_create(sets):
    out = ""
    temp = ""
    for set_num,taskset in enumerate(sets):
        for task_num, task in enumerate(taskset):
            temp = "xTaskCreateCheckpointed( T{}_{}, ".format(set_num,task_num)
            temp += "(signed char *) \"T{}_{}\", ".format(set_num,task_num)
            temp += "configMINIMAL_STACK_SIZE, "
            temp += "(void*) &d{}_{}, ".format(set_num,task_num)
            temp += "1, "
            temp += "&xT{}_{}, ".format(set_num,task_num)
            temp += "pdFALSE, "
            temp += "NULL, "
            temp += "d{}_{}, ".format(set_num,task_num)
            temp += "NULL"
            temp += ");\n"
            out += temp
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

def write_main_func(sets):
    out = "int main_gen(void) {\n"
    creates = write_task_create(sets)
    creates_indent = re.sub( '^','\t', creates ,flags=re.MULTILINE )
    out += creates_indent + "\n"
    out += "\tvTaskStartScheduler();\n"
    out += "\tfor(;;);\n"
    out += "}\n\n"
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

if __name__ == "__main__":
    if (len(sys.argv) < 3):
        print("Must provide input taskset file and output C file names")
        exit()
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]

    output_fd = open(output_file, "w")

    sets = read_tasksets_file(input_file)

    file_header = write_file_header()
    task_handles = write_task_handles(sets)
    task_deadlines = write_task_deadlines(sets)
    func_header_stubs = write_task_func_headers(sets)
    main_func = write_main_func(sets)
    task_functions = write_task_funcs(sets)
    
    out_string = file_header + task_handles + task_deadlines + \
                    func_header_stubs + main_func + task_functions + "\n"
    
    output_fd.write(out_string)
    output_fd.close()

