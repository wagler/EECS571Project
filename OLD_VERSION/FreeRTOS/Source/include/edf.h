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
	TickType_t releaseTime; // may delete later
	BaseType_t isDone; // task has completed an execution for this period
	uint32_t priorityNumber; // needed for TCB switching
} taskData_t;


typedef struct nodes {
	taskData_t td;
	struct node_t* next;
} node_t;

static uint32_t edf_currentPriorityNumber;
static node_t* edf_taskDataList; // head of linked list

// adds to the front right now. if that has to change later its nbd
// the decision to use a taskData instead of taskData* const was purposeful
void project_listPush(taskData_t data, node_t* head);

//NOTE: for EDF, tasks only release at the beginning of their periods, so we need some way
// of resetting the isDone status of completed tasks once the system tick count reaches
// their release time

// function that tasks will call in order to set their done flag to true
//void edf_markAsDone();

// this function should traverse edf_taskDataList and find tasks that are ready to release
void edf_releaseTasks();

// vPortYieldFromISR in port.c gets task handles then feeds them to prvGetThreadFromTask
// so this needs to return the handle of the task that should run next
// this should run from the scheduler (probably in vTaskSwitchContext())
uint32_t edf_getNextTask();
	// scan through edf_taskDataList and determine which task has the nearest deadline
	// should be able to do so using only that list and xTaskGetTickCount()
	// NOTE: need to make sure xTaskGetTickCount can be ran from inside scheduler
	// NOTE: make sure that idle task can still run using this scheme
	// NOTE: edf_taskDataList is assumed to not be empty


/* BASEIC SCHEDULER LAYOUT

	1. scan edf_task list and see if a context switch is needed
	2. if it is, find the next task and switch to that
	3. if not, run idle task until a task releases
*/


#endif /* INC_EDF_H */
