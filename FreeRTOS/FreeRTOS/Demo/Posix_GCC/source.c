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

const unsigned long d0 = 54;
const unsigned long d1 = 204;
const unsigned long d1b = 68;
const unsigned long d2 = 213;
const unsigned long d3 = 162;

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
	
	printf("T0: avg runtime=0, period=54, squshable=False\n");
	printf("T1: avg runtime=70, period=204, squshable=True\n");
	printf("T2: avg runtime=61, period=213, squshable=False\n");
	printf("T3: avg runtime=28, period=162, squshable=False\n");
	vTaskStartScheduler();
	for(;;);
}

//Task set #0, Task #0
//Deadline: 54, Avg Exe Time: 0
static void T0(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    for(;;)
    {   
        job++;
        printf("@%ld\t Started executing T0, Job %d. Deadline: %ld\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 54) + 1) * 54);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 0*10);
        printf("@%ld\t T0, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}

//Task set #0, Task #1
//Deadline: 204, Avg Exe Time: 70
static void T1(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    const uint32_t avg_rt = 70;
    uint32_t rt_bound = avg_rt;
    for(;;)
    {   
        getcontext(&backupContextT1);
        job++;
        printf("@%ld\t Started executing T1, Job %d. Deadline: %ld\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 204) + 1) * 204);

        rt_bound = avg_rt;
        if ((rand()%100) > 80) {
            rt_bound = (uint32_t)(204/2) + 2;
        }
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < rt_bound*10);
        printf("@%ld\t T1, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}

//Task set #0, Task #1
//Deadline: 68, Avg Exe Time: 23
static void T1b(void *pvParameters) {

    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    vTaskSuspend(xT1b);
    for(;;)
    {   
        printf("@%ld\t Started executing T1b. Deadline: %ld\n", xTaskGetTickCount(), ((xTaskGetTickCount() / 68) + 1) * 68);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 23*10);
        printf("@%ld\t T1b is complete\n", xTaskGetTickCount());
        vTaskSuspend(xT1b);
    }
}

//Task set #0, Task #2
//Deadline: 213, Avg Exe Time: 61
static void T2(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    for(;;)
    {   
        job++;
        printf("@%ld\t Started executing T2, Job %d. Deadline: %ld\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 213) + 1) * 213);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 61*10);
        printf("@%ld\t T2, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}

//Task set #0, Task #3
//Deadline: 162, Avg Exe Time: 28
static void T3(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    for(;;)
    {   
        job++;
        printf("@%ld\t Started executing T3, Job %d. Deadline: %ld\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 162) + 1) * 162);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 28*10);
        printf("@%ld\t T3, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}


