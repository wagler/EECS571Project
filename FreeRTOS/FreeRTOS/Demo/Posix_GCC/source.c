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


xTaskHandle xT0, xT0b, xT1, xT1b, xT2, xT2b, xT3, xT3b, xT4, xT4b, xT5;

const unsigned long d0 = 82;
const unsigned long d0b = 27;
const unsigned long d1 = 103;
const unsigned long d1b = 34;
const unsigned long d2 = 214;
const unsigned long d2b = 71;
const unsigned long d3 = 126;
const unsigned long d3b = 42;
const unsigned long d4 = 61;
const unsigned long d4b = 20;
const unsigned long d5 = 218;

ucontext_t backupContextT0;
ucontext_t backupContextT1;
ucontext_t backupContextT2;
ucontext_t backupContextT3;
ucontext_t backupContextT4;

static void T0(void *pvParameters);
static void T0b(void *pvParameters);
static void T1(void *pvParameters);
static void T1b(void *pvParameters);
static void T2(void *pvParameters);
static void T2b(void *pvParameters);
static void T3(void *pvParameters);
static void T3b(void *pvParameters);
static void T4(void *pvParameters);
static void T4b(void *pvParameters);
static void T5(void *pvParameters);

int main_gen(void) {
	time_t t;
	srand((unsigned) time(&t));
	xTaskCreateCheckpointed( T0, (signed char *) "T0", configMINIMAL_STACK_SIZE, (void*) &d0, 1, &xT0, pdTRUE, &xT0b, d0/2, &backupContextT0);
	xTaskCreateCheckpointed( T0b, (signed char *) "T0b", configMINIMAL_STACK_SIZE, (void*) &d0b, 1, &xT0b, pdFALSE, NULL, d0b, NULL);
	xTaskCreateCheckpointed( T1, (signed char *) "T1", configMINIMAL_STACK_SIZE, (void*) &d1, 1, &xT1, pdTRUE, &xT1b, d1/2, &backupContextT1);
	xTaskCreateCheckpointed( T1b, (signed char *) "T1b", configMINIMAL_STACK_SIZE, (void*) &d1b, 1, &xT1b, pdFALSE, NULL, d1b, NULL);
	xTaskCreateCheckpointed( T2, (signed char *) "T2", configMINIMAL_STACK_SIZE, (void*) &d2, 1, &xT2, pdTRUE, &xT2b, d2/2, &backupContextT2);
	xTaskCreateCheckpointed( T2b, (signed char *) "T2b", configMINIMAL_STACK_SIZE, (void*) &d2b, 1, &xT2b, pdFALSE, NULL, d2b, NULL);
	xTaskCreateCheckpointed( T3, (signed char *) "T3", configMINIMAL_STACK_SIZE, (void*) &d3, 1, &xT3, pdTRUE, &xT3b, d3/2, &backupContextT3);
	xTaskCreateCheckpointed( T3b, (signed char *) "T3b", configMINIMAL_STACK_SIZE, (void*) &d3b, 1, &xT3b, pdFALSE, NULL, d3b, NULL);
	xTaskCreateCheckpointed( T4, (signed char *) "T4", configMINIMAL_STACK_SIZE, (void*) &d4, 1, &xT4, pdTRUE, &xT4b, d4/2, &backupContextT4);
	xTaskCreateCheckpointed( T4b, (signed char *) "T4b", configMINIMAL_STACK_SIZE, (void*) &d4b, 1, &xT4b, pdFALSE, NULL, d4b, NULL);
	xTaskCreateCheckpointed( T5, (signed char *) "T5", configMINIMAL_STACK_SIZE, (void*) &d5, 1, &xT5, pdFALSE, NULL, d5, NULL);
	
	printf("T0: avg runtime=3, period=82, squshable=True\n");
	printf("T1: avg runtime=45, period=103, squshable=True\n");
	printf("T2: avg runtime=15, period=214, squshable=True\n");
	printf("T3: avg runtime=5, period=126, squshable=True\n");
	printf("T4: avg runtime=1, period=61, squshable=True\n");
	printf("T5: avg runtime=38, period=218, squshable=False\n");
	vTaskStartScheduler();
	for(;;);
}

//Task set #0, Task #0
//Deadline: 82, Avg Exe Time: 3
static void T0(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    const uint32_t avg_rt = 3;
    uint32_t rt_bound = avg_rt;
    for(;;)
    {   
        getcontext(&backupContextT0);
        job++;
        
        rt_bound = avg_rt;
        if ((rand()%100) > 80) {
            rt_bound = (uint32_t)(82/2) + 2;
        }
        
        printf("@%ld\t Started executing T0, Job %d. Deadline: %ld, Runtime: %d\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 82) + 1) * 82,rt_bound);
        
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < rt_bound*10);
        printf("@%ld\t T0, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}

//Task set #0, Task #0
//Deadline: 27, Avg Exe Time: 1
static void T0b(void *pvParameters) {

    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    vTaskSuspend(xT0b);
    for(;;)
    {   
        printf("@%ld\t Started executing T0b. Deadline: %ld\n", xTaskGetTickCount(), ((xTaskGetTickCount() / 27) + 1) * 27);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 1*10);
        printf("@%ld\t T0b is complete\n", xTaskGetTickCount());
        vTaskSuspend(xT0b);
    }
}

//Task set #0, Task #1
//Deadline: 103, Avg Exe Time: 45
static void T1(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    const uint32_t avg_rt = 45;
    uint32_t rt_bound = avg_rt;
    for(;;)
    {   
        getcontext(&backupContextT1);
        job++;
        
        rt_bound = avg_rt;
        if ((rand()%100) > 80) {
            rt_bound = (uint32_t)(103/2) + 2;
        }
        
        printf("@%ld\t Started executing T1, Job %d. Deadline: %ld, Runtime: %d\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 103) + 1) * 103,rt_bound);
        
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < rt_bound*10);
        printf("@%ld\t T1, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}

//Task set #0, Task #1
//Deadline: 34, Avg Exe Time: 15
static void T1b(void *pvParameters) {

    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    vTaskSuspend(xT1b);
    for(;;)
    {   
        printf("@%ld\t Started executing T1b. Deadline: %ld\n", xTaskGetTickCount(), ((xTaskGetTickCount() / 34) + 1) * 34);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 15*10);
        printf("@%ld\t T1b is complete\n", xTaskGetTickCount());
        vTaskSuspend(xT1b);
    }
}

//Task set #0, Task #2
//Deadline: 214, Avg Exe Time: 15
static void T2(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    const uint32_t avg_rt = 15;
    uint32_t rt_bound = avg_rt;
    for(;;)
    {   
        getcontext(&backupContextT2);
        job++;
        
        rt_bound = avg_rt;
        if ((rand()%100) > 80) {
            rt_bound = (uint32_t)(214/2) + 2;
        }
        
        printf("@%ld\t Started executing T2, Job %d. Deadline: %ld, Runtime: %d\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 214) + 1) * 214,rt_bound);
        
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < rt_bound*10);
        printf("@%ld\t T2, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}

//Task set #0, Task #2
//Deadline: 71, Avg Exe Time: 5
static void T2b(void *pvParameters) {

    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    vTaskSuspend(xT2b);
    for(;;)
    {   
        printf("@%ld\t Started executing T2b. Deadline: %ld\n", xTaskGetTickCount(), ((xTaskGetTickCount() / 71) + 1) * 71);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 5*10);
        printf("@%ld\t T2b is complete\n", xTaskGetTickCount());
        vTaskSuspend(xT2b);
    }
}

//Task set #0, Task #3
//Deadline: 126, Avg Exe Time: 5
static void T3(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    const uint32_t avg_rt = 5;
    uint32_t rt_bound = avg_rt;
    for(;;)
    {   
        getcontext(&backupContextT3);
        job++;
        
        rt_bound = avg_rt;
        if ((rand()%100) > 80) {
            rt_bound = (uint32_t)(126/2) + 2;
        }
        
        printf("@%ld\t Started executing T3, Job %d. Deadline: %ld, Runtime: %d\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 126) + 1) * 126,rt_bound);
        
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < rt_bound*10);
        printf("@%ld\t T3, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}

//Task set #0, Task #3
//Deadline: 42, Avg Exe Time: 1
static void T3b(void *pvParameters) {

    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    vTaskSuspend(xT3b);
    for(;;)
    {   
        printf("@%ld\t Started executing T3b. Deadline: %ld\n", xTaskGetTickCount(), ((xTaskGetTickCount() / 42) + 1) * 42);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 1*10);
        printf("@%ld\t T3b is complete\n", xTaskGetTickCount());
        vTaskSuspend(xT3b);
    }
}

//Task set #0, Task #4
//Deadline: 61, Avg Exe Time: 1
static void T4(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    const uint32_t avg_rt = 1;
    uint32_t rt_bound = avg_rt;
    for(;;)
    {   
        getcontext(&backupContextT4);
        job++;
        
        rt_bound = avg_rt;
        if ((rand()%100) > 80) {
            rt_bound = (uint32_t)(61/2) + 2;
        }
        
        printf("@%ld\t Started executing T4, Job %d. Deadline: %ld, Runtime: %d\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 61) + 1) * 61,rt_bound);
        
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < rt_bound*10);
        printf("@%ld\t T4, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}

//Task set #0, Task #4
//Deadline: 20, Avg Exe Time: 0
static void T4b(void *pvParameters) {

    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    vTaskSuspend(xT4b);
    for(;;)
    {   
        printf("@%ld\t Started executing T4b. Deadline: %ld\n", xTaskGetTickCount(), ((xTaskGetTickCount() / 20) + 1) * 20);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 0*10);
        printf("@%ld\t T4b is complete\n", xTaskGetTickCount());
        vTaskSuspend(xT4b);
    }
}

//Task set #0, Task #5
//Deadline: 218, Avg Exe Time: 38
static void T5(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    for(;;)
    {   
        job++;
        printf("@%ld\t Started executing T5, Job %d. Deadline: %ld, Runtime: %d\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 218) + 1) * 218,38);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 38*10);
        printf("@%ld\t T5, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}


