#include "sam.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart.h"
#include "myprintf.h"

/* Priorities at which the tasks are created. */
#define myTASK_TASK_PRIORITY         	( tskIDLE_PRIORITY + 1 )

xQueueHandle Global_Queue_Handle = 0;

void sender_Task(void *p){
	int i = 0;
	while (1) {
		myprintf("Send %d to receiver task\n", i);
		if (!xQueueSend(Global_Queue_Handle, &i, 1000)) {
			myprintf("Failed to send to queue\n");
		}
		++i;
		vTaskDelay(2000);
	}
}

void receiver_Task(void *p){
	int rx_int = 0;
	while (1) {
		if (xQueueReceive(Global_Queue_Handle, &rx_int, 1000)){
			myprintf("Received %d \n", rx_int);
		}
		else {
			myprintf("Failed to receive data from queue \n");
		}
	}
}

int main()
{
	SystemInit();
	/* Switch to 8MHz clock (disable prescaler) */
	
	SYSCTRL->OSC8M.bit.PRESC = 0;
	initUART();
	
	// Create the queue
	Global_Queue_Handle = xQueueCreate(3, sizeof(int));
	xTaskCreate(sender_Task, (const char * const) "tx", 512, NULL, myTASK_TASK_PRIORITY, NULL);
	xTaskCreate(receiver_Task, (const char * const) "rx", 512, NULL, myTASK_TASK_PRIORITY, NULL);
    
	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	for( ;; );
	return(0);
}