import csv
import re
import sys

class Task:
    def __init__(self, runtime, period, utilization, squashable, alt):
        self.period = period
        self.utilization = utilization
        self.runtime = int(runtime)
        self.squashable = squashable
        self.alt = alt
        
def write_task_func(set_num,task_num,rel_deadline, avg_exe_time):

    srccode= """//Task set #{}, Task #{}
//Deadline: {}, Avg Exe Time: {}
static void T{}(void *pvParameters) {{

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    for(;;)
    {{   
        job++;
        printf("@%ld\\t Started executing T{}, Job %d. Deadline: %ld, Runtime: %d\\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / {}) + 1) * {},{});
 
        do {{
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        }} while (rt < {}*10);
        printf("@%ld\\t T{}, Job %d is complete\\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }}
}}\n""".format(
        set_num,
        task_num,
        rel_deadline,
        avg_exe_time,
        task_num,
        task_num,
        rel_deadline,
        rel_deadline,
        avg_exe_time,
        avg_exe_time,
        task_num
    )


    return srccode

def write_task_func_squashable(set_num,task_num,rel_deadline, avg_exe_time):

    srccode= """//Task set #{}, Task #{}
//Deadline: {}, Avg Exe Time: {}
static void T{}(void *pvParameters) {{

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    const uint32_t avg_rt = {};
    uint32_t rt_bound = avg_rt;
    for(;;)
    {{   
        getcontext(&backupContextT{});
        job++;
        
        rt_bound = avg_rt;
        if ((rand()%100) > 80) {{
            rt_bound = (uint32_t)({}/2) + 2;
        }}
        
        printf("@%ld\\t Started executing T{}, Job %d. Deadline: %ld, Runtime: %d\\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / {}) + 1) * {},rt_bound);
        
        do {{
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        }} while (rt < rt_bound*10);
        printf("@%ld\\t T{}, Job %d is complete\\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }}
}}\n""".format(
        set_num,
        task_num,
        rel_deadline,
        avg_exe_time,
        task_num,
        avg_exe_time,
        task_num,
        task_num,
        rel_deadline,
        rel_deadline,
        rel_deadline,
        task_num
    )

    return srccode

def write_task_func_alt(set_num,task_num,rel_deadline, avg_exe_time):

    srccode= """//Task set #{}, Task #{}
//Deadline: {}, Avg Exe Time: {}
static void T{}b(void *pvParameters) {{

    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    vTaskSuspend(xT{}b);
    for(;;)
    {{   
        printf("@%ld\\t Started executing T{}b. Deadline: %ld\\n", xTaskGetTickCount(), ((xTaskGetTickCount() / {}) + 1) * {});
 
        do {{
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        }} while (rt < {}*10);
        printf("@%ld\\t T{}b is complete\\n", xTaskGetTickCount());
        vTaskSuspend(xT{}b);
    }}
}}\n""".format(
        set_num,
        task_num,
        rel_deadline,
        avg_exe_time,
        task_num,
        task_num,
        task_num,
        rel_deadline,
        rel_deadline,
        avg_exe_time,
        task_num,
        task_num
    )

    return srccode

def write_task_funcs(sets):
    out = ""
    for set_num,s in enumerate(sets):
        for task_num,task in enumerate(s):
            if (task.squashable):
                prog = write_task_func_squashable(set_num, task_num, task.period, task.runtime)
                prog += "\n"
                prog += write_task_func_alt(set_num, task_num, task.alt.period, task.alt.runtime)
            else:
                prog = write_task_func(set_num, task_num, task.period, task.runtime)
            out += prog
            out += "\n"
    return out

def write_ucontexts(sets):
    out = ""
    for s in sets:
        count = 0
        for task in s:
            if (task.squashable):
                out += "ucontext_t backupContextT{};\n".format(count)
            count+=1
    out += "\n"
    return out

def write_task_func_headers(sets):
    out = ""
    for set_num,taskset in enumerate(sets):
        for task_num,task in enumerate(taskset):
            out += "static void T{}(void *pvParameters);\n".format(task_num)
            if (task.squashable):
                out += "static void T{}b(void *pvParameters);\n".format(task_num)
    out += "\n"
    return out

def write_task_handles(sets):
    out = "xTaskHandle "
    for set_num, s in enumerate(sets):
        for task_num, task in enumerate(s):
            out += "xT{}".format(task_num)
            if (task.squashable):
                out += ", xT{}b".format(task_num)
            if not (task_num==(len(s)-1) and set_num==(len(sets)-1)):
                out += ", "
    out += ";\n\n"
    return out

def write_task_deadlines(sets):
    out = ""
    temp = ""
    for s in sets:
        for task_num, task in enumerate(s):
            temp = "const unsigned long d{}".format(task_num)
            temp += " = {};\n".format(task.period) #set the deadline
            out += temp
            if (task.squashable):
                temp = "const unsigned long d{}b".format(task_num)
                temp += " = {};\n".format(task.alt.period) #set the deadline
                out += temp
    out += "\n"
    return out

def write_task_create(task_num):
    out = "xTaskCreateCheckpointed( T{}, ".format(task_num)
    out += "(signed char *) \"T{}\", ".format(task_num)
    out += "configMINIMAL_STACK_SIZE, "
    out += "(void*) &d{}, ".format(task_num)
    out += "1, "
    out += "&xT{}, ".format(task_num)
    out += "pdFALSE, "
    out += "NULL, "
    out += "d{}, ".format(task_num)
    out += "NULL"
    out += ");\n"
    return out

def write_task_create_alt(task_num):
    out = "xTaskCreateCheckpointed( T{}b, ".format(task_num)
    out += "(signed char *) \"T{}b\", ".format(task_num)
    out += "configMINIMAL_STACK_SIZE, "
    out += "(void*) &d{}b, ".format(task_num)
    out += "1, "
    out += "&xT{}b, ".format(task_num)
    out += "pdFALSE, "
    out += "NULL, "
    out += "d{}b, ".format(task_num)
    out += "NULL"
    out += ");\n"
    return out

def write_task_create_squashable(task_num):
    out = "xTaskCreateCheckpointed( T{}, ".format(task_num)
    out += "(signed char *) \"T{}\", ".format(task_num)
    out += "configMINIMAL_STACK_SIZE, "
    out += "(void*) &d{}, ".format(task_num)
    out += "1, "
    out += "&xT{}, ".format(task_num)
    out += "pdTRUE, "
    out += "&xT{}b, ".format(task_num)
    out += "d{}/2, ".format(task_num)
    out += "&backupContextT{}".format(task_num)
    out += ");\n"
    out += write_task_create_alt(task_num)
    return out

def write_task_creates(sets):
    out = ""
    for s in sets:
        for task_num, task in enumerate(s):
            if (task.squashable):
                out += write_task_create_squashable(task_num)
            else:
                out += write_task_create(task_num)
    return out

def write_file_header():
    out = """#include <stdio.h>
#include <stdlib.h>
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
    out += "\ttime_t t;\n"
    out += "\tsrand((unsigned) time(&t));\n"
    creates = write_task_creates(sets)
    creates_indent = re.sub( '^','\t', creates ,flags=re.MULTILINE )
    out += creates_indent + "\n"
    for s in sets:
        for task_num, task in enumerate(s):
            out += "\tprintf(\"T{}: avg runtime={}, period={}, squshable={}\\n\");\n".format(task_num, task.runtime, task.period, task.squashable)
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
            if (row[0]=='s'):
                t1 = Task(float(row[1]), int(row[2]), float(row[3]), True, False)
                curr_set.append(t1)
            elif (row[0]=='a'):
                t1 = Task(float(row[1]), int(row[2]), float(row[3]), False, False)
                curr_set[-1].alt = t1
            elif(row[0]=='n'):
                t1 = Task(float(row[1]), int(row[2]), float(row[3]), False, False)
                curr_set.append(t1)
            else:
                print("Error")
            
    sets.append(curr_set)
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
    ucontexts = write_ucontexts(sets)
    func_header_stubs = write_task_func_headers(sets)
    main_func = write_main_func(sets)
    task_functions = write_task_funcs(sets)
    
    out_string = file_header + task_handles + task_deadlines + \
                    ucontexts + func_header_stubs + main_func + task_functions + "\n"
    
    output_fd.write(out_string)
    output_fd.close()
    
            

