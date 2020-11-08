#include <stdlib.h>

#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "edf.h"

void project_listPush(taskData_t data, node_t* head) {
	node_t* newNode = (node_t*)pvPortMalloc(sizeof(node_t));
	newNode->next = edf_taskDataList;
	edf_taskDataList = newNode;
	edf_taskDataList->td = data; // NOTE may need to do deep copy?

	// DEBUG
//	console_print("new head period: %d\n", edf_taskDataList->td.period); 
}

void edf_markTaskDone(TaskHandle_t doneTask) {
	node_t* iter = edf_taskDataList;
	
	//console_print("stuff\n");
	taskENTER_CRITICAL();
	{
		//console_print("stuff\n");
		while(iter) {
			if(iter->td.handle == doneTask) {
			//	console_print("marking task %d\n", doneTask);
				iter->td.isDone = pdTRUE;
			}
			iter = iter->next;
		}
	}
	taskEXIT_CRITICAL();
	//vPortYieldFromISR();
}


void edf_releaseTasks() {
	node_t* iter = edf_taskDataList;
	
	TickType_t currentTick = xTaskGetTickCount();

	while(iter) {
		//console_print("\t\ttask %d period: %d release time: %d done: %d tick: %d\n", iter->td.handle, iter->td.period, iter->td.releaseTime, iter->td.isDone, currentTick);
		if(iter->td.isDone == pdTRUE) {
			if(iter->td.releaseTime + iter->td.period <= currentTick) { 
		//		console_print("task released with period %d\n", iter->td.period);
				iter->td.isDone = pdFALSE;
				iter->td.releaseTime = currentTick;
				vTaskResume(iter->td.handle);
			}
		}
		iter = iter->next;
	}
	//console_print("\n\n");
}

uint32_t edf_getNextTask() {
	node_t* iter = edf_taskDataList;
	TaskHandle_t nextTaskHandle = 0; // placeholder. NOTE make sure idle task can still run
	uint32_t nextPriorityNumber = 0;
	uint64_t earliestDeadlineDist = -1; // will overflow to uint64_t INTMAX 
	int64_t slack = 0;

	//console_print("test%d\n", (uint64_t)pxReadyTasksLists);
	//console_print("test%d\n", (uint64_t)&pxCurrentTCB);


	TickType_t currentTick = xTaskGetTickCount();

	while(iter) {
		//console_print("\t\t\ttask %d period: %d release time: %d done: %d tick: %d priority: %d\n", iter->td.handle, iter->td.period, iter->td.releaseTime, iter->td.isDone, currentTick, iter->td.priorityNumber);
		if(iter->td.isDone == pdFALSE) {
			// task needs more time
			// NOTE: rollover?
			slack = (iter->td.period + iter->td.releaseTime) - currentTick;
			if((uint64_t)slack < earliestDeadlineDist) {
				// found next task
				nextTaskHandle = iter->td.handle;
				nextPriorityNumber = iter->td.priorityNumber;
				earliestDeadlineDist = (uint64_t)slack;
			}		
			// NOTE: negative slack?	
		}
		iter = iter->next;
	}
	edf_currentPriorityNumber = nextPriorityNumber;
	return nextPriorityNumber; 
}
