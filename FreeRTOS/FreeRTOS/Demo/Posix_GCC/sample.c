#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h" 


xTaskHandle xT0_0, xT0_1, xT0_2;

const unsigned long d0_0 = 74;
const unsigned long d0_1 = 274;
const unsigned long d0_2 = 116;

static void T0_0(void *pvParameters);
static void T0_1(void *pvParameters);
static void T0_2(void *pvParameters);

int main_gen(void) {
	xTaskCreateCheckpointed( T0_0, (signed char *) "T0_0", configMINIMAL_STACK_SIZE, (void*) &d0_0, 1, &xT0_0, pdFALSE, NULL, d0_0, NULL);
	xTaskCreateCheckpointed( T0_1, (signed char *) "T0_1", configMINIMAL_STACK_SIZE, (void*) &d0_1, 1, &xT0_1, pdFALSE, NULL, d0_1, NULL);
	xTaskCreateCheckpointed( T0_2, (signed char *) "T0_2", configMINIMAL_STACK_SIZE, (void*) &d0_2, 1, &xT0_2, pdFALSE, NULL, d0_2, NULL);
	
	vTaskStartScheduler();
	for(;;);
}

//Task set #0, Task #0
//Deadline: 74, Avg Exe Time: 6
static void T0_0(void *pvParameters) {
    unsigned int job = 0;
    unsigned long end;
    for(;;)
    {   
        job++;
    	end = (xTaskGetTickCount() / 74) * 74 + 6;
		printf("Executing T0 start: %lu deadline: %ld job: %d \n", xTaskGetTickCount(), ((xTaskGetTickCount() / 74) + 1) * 74, job);
        while(xTaskGetTickCount() < end) { }

        vTaskDelay(1);
    }
}


//Task set #0, Task #1
//Deadline: 274, Avg Exe Time: 51
static void T0_1(void *pvParameters) {
    unsigned int job = 0;
    unsigned long end;
    for(;;)
    {   
        job++;
    	end = (xTaskGetTickCount() / 274) * 274 + 51;
		printf("Executing T1 start: %lu deadline: %ld job: %d \n", xTaskGetTickCount(), ((xTaskGetTickCount() / 274) + 1) * 274, job);
        while(xTaskGetTickCount() < end) { }

        vTaskDelay(1);
    }
}


//Task set #0, Task #2
//Deadline: 116, Avg Exe Time: 62
static void T0_2(void *pvParameters) {
    unsigned int job = 0;
    unsigned long end;
    for(;;)
    {   
        job++;
    	end = (xTaskGetTickCount() / 116) * 116 + 62;
		printf("Executing T2 start: %lu deadline: %ld job: %d \n", xTaskGetTickCount(), ((xTaskGetTickCount() / 116) + 1) * 116, job);
        while(xTaskGetTickCount() < end) { }

        vTaskDelay(1);
    }
}



