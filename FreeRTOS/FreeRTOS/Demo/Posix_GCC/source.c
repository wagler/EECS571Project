#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h" 


xTaskHandle xT0, xT1, xT2;

const unsigned long d0 = 74;
const unsigned long d1 = 274;
const unsigned long d2 = 116;

static void T0(void *pvParameters);
static void T1(void *pvParameters);
static void T2(void *pvParameters);

int main_gen(void) {
	xTaskCreateCheckpointed( T0, (signed char *) "T0", configMINIMAL_STACK_SIZE, (void*) &d0, 1, &xT0, pdFALSE, NULL, d0, NULL);
	xTaskCreateCheckpointed( T1, (signed char *) "T1", configMINIMAL_STACK_SIZE, (void*) &d1, 1, &xT1, pdFALSE, NULL, d1, NULL);
	xTaskCreateCheckpointed( T2, (signed char *) "T2", configMINIMAL_STACK_SIZE, (void*) &d2, 1, &xT2, pdFALSE, NULL, d2, NULL);
	
	vTaskStartScheduler();
	for(;;);
}

//Task set #0, Task #0
//Deadline: 74, Avg Exe Time: 6
static void T0(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    for(;;)
    {   
        job++;
        printf("@%ld\t Started executing T0, Job %d. Deadline: %ld\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 74) + 1) * 74);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 6*10);
        printf("@%ld\t T0, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}

//Task set #0, Task #1
//Deadline: 274, Avg Exe Time: 51
static void T1(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    for(;;)
    {   
        job++;
        printf("@%ld\t Started executing T1, Job %d. Deadline: %ld\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 274) + 1) * 274);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 51*10);
        printf("@%ld\t T1, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}

//Task set #0, Task #2
//Deadline: 116, Avg Exe Time: 62
static void T2(void *pvParameters) {

    unsigned int job = 0;
    uint32_t rt = 0;
    TaskStatus_t tsk_sts;
    for(;;)
    {   
        job++;
        printf("@%ld\t Started executing T2, Job %d. Deadline: %ld\n", xTaskGetTickCount(), job, ((xTaskGetTickCount() / 116) + 1) * 116);
 
        do {
            vTaskGetInfo(NULL,&tsk_sts,pdFALSE,eRunning);
            rt = (uint32_t)((tsk_sts.ulRunTimeCounter));
        } while (rt < 62*10);
        printf("@%ld\t T2, Job %d is complete\n", xTaskGetTickCount(), job);
        vTaskDelay(1);
    }
}


