import csv

def write_task_func(task_num,rel_deadline):

    srccode="""static void T{}( void *pvParameters )
    {
        unsigned int job = 0;
        unsigned long end;
        while(1)
        {   
            job++;
            end = xTaskGetTickCount() + {}
            
            printf("Executing T{} start: %lu deadline: %d job: %d \n", xTaskGetTickCount(), ((xTaskGetTickCount() / {}) + 1) * {}, job);

            while(xTaskGetTickCount() < end) \{ \}

            // Doesn't matter what val we input, because
            // vTaskDelay already knows how much time is left
            // until the task's deadline
            vTaskDelay(99/portTICK_RATE_MS);
        }
    }""".format(task_num,rel_deadline,task_num,rel_deadline,rel_deadline)

set_count = 0
last_was_empty_line = True
with open('tasksets.txt', newline='') as f:
    reader = csv.reader(f)
    next(reader)
    for row in reader:
        if (len(row)==0):
            last_was_empty_line=True
            continue

        if (last_was_empty_line):
            print("==========Set ", set_count, "==========")
            set_count+=1
            last_was_empty_line=False

        print("Runtime: ", row[0], "\tPeriod: ", row[1], "\tUtilization: ", row[2], "\n")