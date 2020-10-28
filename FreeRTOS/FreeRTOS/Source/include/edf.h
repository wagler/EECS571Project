#ifndef INC_EDF_H
#define INT_EDF_H

#ifndef INC_FREERTOS_H
    #error "include FreeRTOS.h must appear in source files before include task.h"
#endif

#include "list.h"
#include "task.h"

typedef struct taskData {
	TaskHandle_t handle;
	uint32_t period;
	uint32_t deadline; // may delete later
	uint8_t isDone; // task has completed an execution for this period
} taskData_t;


typedef struct nodes {
	taskData_t td;
	struct node_t* next;
} node_t;

static node_t* edf_taskDataList; // head of linked list

// adds to the front right now. if that has to change later its nbd
// the decision to use a taskData instead of taskData* const was purposeful
void project_listPush(taskData_t data, node_t* head);

// vPortYieldFromISR in port.c gets task handles then feeds them to prvGetThreadFromTask
// so this needs to return the handle of the task that should run next
// this should run from the scheduler (probably in vTaskSwitchContext())
TaskHandle_t edf_getNextTask();
	// scan through edf_taskDataList and determine which task has the nearest deadline
	// should be able to do so using only that list and xTaskGetTickCount()
	// NOTE: need to make sure xTaskGetTickCount can be ran from inside scheduler


#endif /* INC_EDF_H */
