#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h" 


xTaskHandle xT0, xT1, xT1b, xT2, xT3;

const unsigned long d0 = 64;
const unsigned long d1 = 238;
const unsigned long d1b = 79;
const unsigned long d2 = 227;
const unsigned long d3 = 250;

ucontext_t backupContextT1;

static void T0(void *pvParameters);
static void T1(void *pvParameters);
static void T1b(void *pvParameters);
static void T2(void *pvParameters);
static void T3(void *pvParameters);

int main_gen(void) {
	time_t t;
	srand((unsigned) time(&t));
	xTaskCreateCheckpointed( T0, (signed char *) "T0", configMINIMAL_STACK_SIZE, (void*) &d0, 1, &xT0, pdFALSE, NULL, d0, NULL);
	xTaskCreateCheckpointed( T1, (signed char *) "T1", configMINIMAL_STACK_SIZE, (void*) &d1, 1, &xT1, pdTRUE, &xT1b, d1/2, &backupContextT1);
	xTaskCreateCheckpointed( T1b, (signed char *) "T1b", configMINIMAL_STACK_SIZE, (void*) &d1b, 1, &xT1b, pdFALSE, NULL, d1b, NULL);
	xTaskCreateCheckpointed( T2, (signed char *) "T2", configMINIMAL_STACK_SIZE, (void*) &d2, 1, &xT2, pdFALSE, NULL, d2, NULL);
	xTaskCreateCheckpointed( T3, (signed char *) "T3", configMINIMAL_STACK_SIZE, (void*) &d3, 1, &xT3, pdFALSE, NULL, d3, NULL);
	
	printf("T0: avg runtime=9, period=64, squshable=False\n");
	printf("T1: avg runtime=71, period=238, squshable=True\n");
	printf("T2: avg runtime=6, period=227, squshable=False\n");
	printf("T3: avg runtime=84, period=250, squshable=False\n");
	vTaskStartScheduler();
	for(;;);
}

//Task set #0, Task #0
//Deadline: 64, Avg Exe Time: 9
static void T0(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    for(;;)
    {   
        job++;
        printf("@%ld\t Started executing T0, Job %d. Deadline: %ld, Runtime: %d\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 64) + 1) * 64,9);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 9*10);
        printf("@%ld\t T0, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}

//Task set #0, Task #1
//Deadline: 238, Avg Exe Time: 71
static void T1(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    const uint32_t avg_rt = 71;
    uint32_t rt_bound = avg_rt;
    for(;;)
    {   
        getcontext(&backupContextT1);
        job++;

        rt_bound = avg_rt;
        if ((rand()%100) > 80) {
            rt_bound = (uint32_t)(238/2) + 2;
        }
        printf("@%ld\t Started executing T1, Job %d. Deadline: %ld, Runtime: %d\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 238) + 1) * 238, rt_bound);

        
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < rt_bound*10);
        printf("@%ld\t T1, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}

//Task set #0, Task #1
//Deadline: 79, Avg Exe Time: 23
static void T1b(void *pvParameters) {

    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    vTaskSuspend(xT1b);
    for(;;)
    {   
        printf("@%ld\t Started executing T1b. Deadline: %ld, Runtime: %d\n", xTaskGetTickCount(), ((xTaskGetTickCount() / 79) + 1) * 79, 23);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 23*10);
        printf("@%ld\t T1b is complete\n", xTaskGetTickCount());
        vTaskSuspend(xT1b);
    }
}

//Task set #0, Task #2
//Deadline: 227, Avg Exe Time: 6
static void T2(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    for(;;)
    {   
        job++;
        printf("@%ld\t Started executing T2, Job %d. Deadline: %ld, Runtime: %d\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 227) + 1) * 227, 6);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 6*10);
        printf("@%ld\t T2, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}

//Task set #0, Task #3
//Deadline: 250, Avg Exe Time: 84
static void T3(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    for(;;)
    {   
        job++;
        printf("@%ld\t Started executing T3, Job %d. Deadline: %ld, Runtime: %d\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 250) + 1) * 250, 84);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 84*10);
        printf("@%ld\t T3, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}


