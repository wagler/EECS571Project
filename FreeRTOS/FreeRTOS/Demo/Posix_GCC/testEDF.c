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
xTaskHandle xT1;
xTaskHandle xT2,xT3,xT4;

static void T1( void *pvParameters );
static void T2( void *pvParameters );
static void T3( void *pvParameters );
static void T4( void *pvParameters );



const unsigned long dT1 = 100;
const unsigned long dT2 = 70;
const unsigned long dT3 = 50;
const unsigned long dT4 = 400;

int main_blinky(void)
{
	printf("running\n");		
          xTaskCreate( T1, ( signed char * ) "T1", configMINIMAL_STACK_SIZE, (void *)&dT1, 1 , &xT1 );		
	printf("running\n");		
	  xTaskCreate( T2, ( signed char * ) "T2", configMINIMAL_STACK_SIZE, (void *)&dT2, 1 , &xT2 );
	printf("running\n");		
  	xTaskCreate( T3, ( signed char * ) "T3", configMINIMAL_STACK_SIZE, (void *)&dT3, 1 , &xT3 );
	printf("running\n");		
	  xTaskCreate( T4, ( signed char * ) "T4", configMINIMAL_STACK_SIZE, (void *)&dT4, 1 , &xT4 );
	printf("running\n");		
		/* Start the tasks running. */
	vTaskStartScheduler();
	
	for( ;; );
}

/*-----------------------------------------------------------*/

static void T1( void *pvParameters )
{
	unsigned int i = 0;
	while(1)
	{	
		//i = 0xFFFFFFFE + 0xA;
		//printf("%x\n", i);
	 printf("T1 Executing\n");
	 for(i = 0;i < 9000; i++);
	 vTaskDelay( 10 / portTICK_RATE_MS );	
  }
}


static void T2( void *pvParameters )
{
	unsigned long i = 0; 
	while(1)
	{
		printf("T2 executing\n");
		for(i = 0;i < 9000; i++);
		vTaskDelay( 20 / portTICK_RATE_MS);
	}
}

static void T3( void *pvParameters )
{
	int i = 0;
	while(1)
	{	
	 printf("T3 Executing\n");
	 for(i = 0;i < 9000; i++);
	 vTaskDelay( 30 / portTICK_RATE_MS );	
  }
}


static void T4( void *pvParameters )
{
	unsigned long i = 0; 
	while(1)
	{
		printf("T4 executing\n");
	  for(i = 0;i < 9000; i++);
		vTaskDelay( 40 / portTICK_RATE_MS);
	}
}
