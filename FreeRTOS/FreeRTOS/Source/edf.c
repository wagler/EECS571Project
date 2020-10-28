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
	return 0;
}
