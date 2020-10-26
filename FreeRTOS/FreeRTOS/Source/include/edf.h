#ifndef INC_EDF_H
#define INT_EDF_H

#ifndef INC_FREERTOS_H
    #error "include FreeRTOS.h must appear in source files before include task.h"
#endif

#include "list.h"
#include "task.h"


static List_t* projectEDFData_isDone;
static List_t* projectEDFData_periods;


void edfInitLists() {
	projectEDFData_isDone = (List_t*)pvPortMalloc(sizeof(List_t));
	projectEDFData_periods = (List_t*)pvPortMalloc(sizeof(List_t));
}

void edfAddTaskToLists() {
	ListItem_t* newItem = (ListItem_t*)pvPortMalloc(sizeof(ListItem_t));
	vListInitialiseItem(newItem);
	newItem->xItemValue = 123;
	/*console_print("ITEMS1: %d", listCURRENT_LIST_LENGTH(projectEDFData_periods));
	vListInsertEnd(projectEDFData_periods, newItem);
	console_print("ITEMS2: %d", listCURRENT_LIST_LENGTH(projectEDFData_periods));
	console_print("items? %d", projectEDFData_periods->uxNumberOfItems);
	*/
}


/*
TickType_t edfGetTaskPeriod() {
	# traverse project
}


BaseType_t edfGetTaskDone() {

	return pdTRUE // or pdFALSE
}
*/



#endif /* INC_EDF_H */
