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
	console_print("new head period: %d\n", edf_taskDataList->td.period); 
}

TaskHandle_t edf_getNextTask() {
	node_t* iter = edf_taskDataList;
	TaskHandle_t nextTaskHandle = 0; // placeholder. NOTE make sure idle task can still run
	uint64_t earliestDeadlineDist = -1; // will overflow to uint64_t INTMAX 
	int64_t slack = 0;

	TickType_t currentTick = xTaskGetTickCount();

	while(iter) {
		if(iter->td.isDone == pdFALSE) {
			// task needs more time
			slack = (iter->td.period + iter->td.releaseTime) - currentTick;
			if(slack > -1 && (uint64_t)slack < earliestDeadlineDist) {
				// found next task
				nextTaskHandle = iter->td.handle;
				earliestDeadlineDist = (uint64_t)slack;
			}			
		}
		iter = iter->next;
	}

	return nextTaskHandle; 
}
