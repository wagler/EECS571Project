#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h"

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
xTaskHandle xT1,xT2,xT3,xT4;

static void T1( void *pvParameters );
static void T2( void *pvParameters );
static void T3( void *pvParameters );
static void T4( void *pvParameters );

const unsigned long dT1 = 200;
const unsigned long dT2 = 200;
const unsigned long dT3 = 300;
const unsigned long dT4 = 400;

ucontext_t backupContext;

int main_blinky(void)
{
	/********************************/
	// FOR TESTING BACKUP TASKS
	xTaskCreateCheckpointed( T1, ( signed char * ) "T1", configMINIMAL_STACK_SIZE, (void *)&dT1, 1, &xT1, pdFALSE, NULL, dT1, NULL); // backup for T2		
	xTaskCreateCheckpointed( T2, ( signed char * ) "T2", configMINIMAL_STACK_SIZE, (void *)&dT2, 1, &xT2, pdTRUE, &xT1, dT2/2, &backupContext);
	xTaskCreateCheckpointed( T3, ( signed char * ) "T3", configMINIMAL_STACK_SIZE, (void *)&dT3, 1, &xT3, pdFALSE, NULL, dT3, NULL);
	//xTaskCreateCheckpointed( T4, ( signed char * ) "T4", configMINIMAL_STACK_SIZE, (void *)&dT4, 1, &xT4, pdFALSE, NULL, dT4, NULL);
	/********************************/

	/*
	xTaskCreate( T1, ( signed char * ) "T1", configMINIMAL_STACK_SIZE, (void *)&dT1, 1 , &xT1 );		
	xTaskCreate( T2, ( signed char * ) "T2", configMINIMAL_STACK_SIZE, (void *)&dT2, 1 , &xT2 );
	xTaskCreate( T3, ( signed char * ) "T3", configMINIMAL_STACK_SIZE, (void *)&dT3, 1 , &xT3 );
	xTaskCreate( T4, ( signed char * ) "T4", configMINIMAL_STACK_SIZE, (void *)&dT4, 1 , &xT4 );
	*/

	/* Start the tasks running. */
	vTaskStartScheduler();
	
	for( ;; );
}

/*-----------------------------------------------------------*/

static void T1( void *pvParameters )
{
	unsigned int i = 0;
	unsigned int j = 0;
	printf("T1 Started, about to suspend...\n", xT1);
	vTaskSuspend(xT1); // self-suspend
	while(1)
	{	
		printf("T1 Executing %lu deadline: %d job: %d\n", xTaskGetTickCount(), xT1->ulAbsDeadline, j);
		++j;
	 	for(i = 0;i < 9000000; i++) {
			 if(i == 0) {
				 //printf("t1 thread: %lu\n", pthread_self());
			 }
		 }
	 	vTaskSuspend(xT1); // self-suspend
		//vTaskDelay( 100 / portTICK_RATE_MS );	
  }
}


static void T2( void *pvParameters )
{
	unsigned long i = 0; 
	unsigned int j = 0;
	while(1)
	{
		getcontext(&backupContext);
		
		// MIXED
		int itervar = 90000000;
		itervar += (rand() % 10) * itervar;
		
		// SLOW
		//int itervar = 900000000;

		// FAST
		//int itervar = 90000000;
		
		printf("T2 Executing %lu deadline: %d job: %d ITER: %d\n", xTaskGetTickCount(), xT2->ulAbsDeadline, j, itervar);
	 	++j;
		for(i = 0;i < itervar; i++) {
			if(i == 0) {
				//printf("t2 thread: %lu\n", pthread_self());
			}
		}
		vTaskDelay( 200 / portTICK_RATE_MS);
	}
}

static void T3( void *pvParameters )
{
	int i = 0;
	unsigned int j = 0;
	while(1)
	{	
	 	printf("T3 Executing %lu deadline: %d job: %d\n", xTaskGetTickCount(), xT3->ulAbsDeadline, j);
	 	++j;
	 	for(i = 0;i < 90000000; i++) {
			 if(i == 0) {
				 //printf("t3 thread: %lu\n", pthread_self());
			 }
		 }
	 	vTaskDelay( 300 / portTICK_RATE_MS );	
  }
}


static void T4( void *pvParameters )
{
	unsigned long i = 0; 
	unsigned int j = 0;
	while(1)
	{
	 	printf("T4 Executing %lu deadline: %d job: %d\n", xTaskGetTickCount(), xT4->ulAbsDeadline, j);
	 	++j;
	 	for(i = 0;i < 90000000; i++) {
			 if(i == 0) {
				 //printf("t4 thread: %lu\n", pthread_self());
			 }
		 }
		vTaskDelay( 400 / portTICK_RATE_MS);
	}
}
